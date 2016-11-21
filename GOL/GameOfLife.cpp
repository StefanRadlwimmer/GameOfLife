#include "GameOfLife.h"
#include <iostream>
#include <fstream>
#include "OpenCLHelper.h"

GameOfLife::GameOfLife(Life* life, int sizeX, int sizeY) : m_sizeY(sizeY), m_sizeX(sizeX), m_life(life), m_buffer(new Life[sizeY * sizeX]), m_platformId(-1), m_deviceId(-1)
{
}

GameOfLife::~GameOfLife()
{
	delete[] m_life;
	delete[] m_buffer;
}

Life* GameOfLife::Simulate(int generations, Mode mode)
{
	switch (mode)
	{
	case OpenCL:
		SimulateOpenCL(generations);
		break;
	case OpenMP:
		SimulateOpenMP(generations);
		break;
	default:
		SimulateSeq(generations);
		break;
	}

	return m_life;
}

void GameOfLife::SetCLSettings(int platformId, int deviceId)
{
	m_platformId = platformId;
	m_deviceId = deviceId;
}

void GameOfLife::SimulateSeq(int generations)
{
	for (int g = 0; g < generations; ++g)
	{
		int lastLine = m_sizeY - 1;

		CheckLine(lastLine, 0, 1);

		for (int y = 1; y < lastLine; ++y)
		{
			CheckLine(y - 1, y, y + 1);
		}

		CheckLine(lastLine - 1, lastLine, 0);

		SwapBuffers();
	}
}

void GameOfLife::SimulateOpenMP(int generations)
{
	for (int g = 0; g < generations; ++g)
	{
		int lastLine = m_sizeY - 1;

#pragma omp parallel 
		{
#pragma omp single
			CheckLine(lastLine, 0, 1);

# pragma omp for nowait
			for (int y = 1; y < lastLine; ++y)
			{
				CheckLine(y - 1, y, y + 1);
			}

#pragma omp single
			CheckLine(lastLine - 1, lastLine, 0);
		}

		SwapBuffers();
	}
}

void GameOfLife::SimulateOpenCL(int generations) const
{
	const std::string KERNEL_FILE = "kernel.cl";
	cl_int err = CL_SUCCESS;
	
	cl::Device device = OpenCLHelper::GetDevice(m_platformId, m_deviceId);
	cl::Context context(device);

	// load and build the kernel
	std::ifstream sourceFile(KERNEL_FILE);
	if (!sourceFile)
	{
		std::cout << "kernel source file " << KERNEL_FILE << " not found!" << std::endl;
		exit(-1);
	}
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
	cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
	cl::Program program = cl::Program(context, source);

	std::vector<cl::Device> devices { device };
#ifdef __CL_ENABLE_EXCEPTIONS
	try
	{
		err = program.build(devices);
	}
	catch (cl::Error error) {
		// error handling
		// if the kernel has failed to compile, print the error log
		std::string s;
		program.getBuildInfo(device, CL_PROGRAM_BUILD_LOG, &s);
		std::cout << s << std::endl;
		program.getBuildInfo(device, CL_PROGRAM_BUILD_OPTIONS, &s);
		std::cout << s << std::endl;

		std::cerr << "ERROR: " << error.what() << "(" << error.err() << ")" << std::endl;
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	}
#else
	err = program.build(devices);
#endif
	cl::CommandQueue queue(context, 0, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	//cl::CommandQueue queue(context, device, 0, &err);

	// input buffers
	cl::Buffer* life = new cl::Buffer(context, CL_MEM_READ_WRITE, m_sizeX * m_sizeY * sizeof(Life), 0, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	// output buffers
	cl::Buffer* buffer = new cl::Buffer(context, CL_MEM_READ_WRITE, m_sizeX * m_sizeY * sizeof(Life), 0, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	// fill buffers
	err = queue.enqueueWriteBuffer(
		*life, // which buffer to write to
		CL_TRUE, // block until command is complete
		0, // offset
		m_sizeX * m_sizeY * sizeof(Life), // size of write 
		m_life); // pointer to input
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	size_t maxWorkGroupSize = 0;
	err = device.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);

	// Run the kernel on specific ND range
	cl::NDRange global;
	cl::NDRange local;
	OpenCLHelper::DetermineBestWorkGroups(device, m_sizeY, m_sizeX, local, global);

	//create kernels
	cl::Kernel checkLife(program, "CheckLife", &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	for (int g = 0; g < generations; ++g)
	{
		err = checkLife.setArg(0, *life);
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
		err = checkLife.setArg(1, *buffer);
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
		err = checkLife.setArg(2, m_sizeY);
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
		err = checkLife.setArg(3, m_sizeX);
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

		// launch add kernel
		err = queue.enqueueNDRangeKernel(checkLife, cl::NullRange, global, local);
		OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

		cl::Buffer* tmp = life;
		life = buffer;
		buffer = tmp;
	}

	// read back result
	err = queue.enqueueReadBuffer(*life, CL_TRUE, 0, m_sizeX * m_sizeY * sizeof(Life), m_life);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
}

void GameOfLife::CheckLine(int ym1, int y, int yp1) const
{
	ym1 *= m_sizeX;
	y *= m_sizeX;
	yp1 *= m_sizeX;

	int lastField = m_sizeX - 1;

	CheckField(ym1, y, yp1, lastField, 0, 1);

	for (int x = 1; x < lastField; ++x)
	{
		CheckField(ym1, y, yp1, x - 1, x, x + 1);
	}

	CheckField(ym1, y, yp1, lastField - 1, lastField, 0);
}

void GameOfLife::CheckField(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1) const
{
	int count = 0;
	if (m_life[ym1Offset + xm1])
		++count;

	if (m_life[ym1Offset + x])
		++count;

	if (m_life[ym1Offset + xp1])
		++count;

	if (m_life[yOffset + xm1])
		++count;

	if (m_life[yOffset + xp1])
		++count;

	if (m_life[yp1Offset + xm1])
		++count;

	if (m_life[yp1Offset + x])
		++count;

	if (m_life[yp1Offset + xp1])
		++count;

	m_buffer[yOffset + x] = (count == 2 && m_life[yOffset + x]) || count == 3;
}

void GameOfLife::SwapBuffers()
{
	Life* tmp = m_life;
	m_life = m_buffer;
	m_buffer = tmp;
}
