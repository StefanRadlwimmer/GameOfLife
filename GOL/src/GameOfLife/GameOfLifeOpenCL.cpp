#include "GameOfLifeOpenCL.h"
#include <fstream>
#include <iostream>

GameOfLifeOpenCL::GameOfLifeOpenCL(Life* life, int sizeX, int sizeY)
	: GameOfLifeBase(life, sizeX, sizeY)
	, m_clLife(nullptr)
	, m_clBuffer(nullptr)
{}

GameOfLifeOpenCL::~GameOfLifeOpenCL()
{
	delete m_clLife;
	delete m_clBuffer;
}

const Life* GameOfLifeOpenCL::Simulate(int generations)
{
	cl_int err = m_kernel.setArg(2, m_sizeY);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	err = m_kernel.setArg(3, m_sizeX);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	for (int g = 0; g < generations; ++g)
	{
		Simulate();

		SwapBuffers();
	}

	// read back result
	err = m_queue.enqueueReadBuffer(*m_clLife, CL_TRUE, 0, m_sizeX * m_sizeY * sizeof(Life), m_life);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	return m_life;
}

bool GameOfLifeOpenCL::Init(cl::Device device)
{
	m_device = device;

	cl_int err = CL_SUCCESS;
	const cl::Context context(m_device, nullptr, nullptr, nullptr, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	
	// load and build the kernel
	std::ifstream sourceFile(KernelFilePath);
	if (!sourceFile)
	{
		std::cerr << "kernel source file " << KernelFilePath << " not found!" << std::endl;
		exit(-1);
	}
	std::string sourceCode(std::istreambuf_iterator<char>(sourceFile), (std::istreambuf_iterator<char>()));
	const cl::Program::Sources source(1, std::make_pair(sourceCode.c_str(), sourceCode.length() + 1));
	cl::Program program = cl::Program(context, source, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	const std::vector<cl::Device> devices{ m_device };
#ifdef __CL_ENABLE_EXCEPTIONS
	try
	{
		err = program.build(devices);
	}
	catch (cl::Error error) {
		// error handling
		// if the kernel has failed to compile, print the error log
		std::string s;
		program.getBuildInfo(m_device, CL_PROGRAM_BUILD_LOG, &s);
		std::cout << s << std::endl;
		program.getBuildInfo(m_device, CL_PROGRAM_BUILD_OPTIONS, &s);
		std::cout << s << std::endl;

		std::cerr << "ERROR: " << error.what() << "(" << error.err() << ")" << std::endl;

		return false;
	}
#else
	err = program.build(devices);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
#endif

	m_queue = cl::CommandQueue(context, 0, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	// input buffers
	m_clLife = new cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, m_sizeX * m_sizeY * sizeof(Life), m_life, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	// output buffers
	m_clBuffer = new cl::Buffer(context, CL_MEM_READ_WRITE, m_sizeX * m_sizeY * sizeof(Life), nullptr, &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	//create kernels
	m_kernel = cl::Kernel(program, "SimulateCell", &err);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	// Run the kernel on specific ND range
	OpenCLHelper::DetermineBestWorkGroups(m_device, m_sizeY, m_sizeX, m_global, m_local);

	return true;
}

void GameOfLifeOpenCL::Simulate()
{
	cl_int err = m_kernel.setArg(0, *m_clLife);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
	err = m_kernel.setArg(1, *m_clBuffer);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);

	// launch add kernel
	err = m_queue.enqueueNDRangeKernel(m_kernel, cl::NullRange, m_global, m_local);
	OpenCLHelper::CheckClError(err, __FILE__, __LINE__);
}

void GameOfLifeOpenCL::SwapBuffers()
{
	std::swap(m_clLife, m_clBuffer);
}
