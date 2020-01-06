#pragma once
#include "GameOfLifeBase.h"
#include "OpenCLHelper.h"

class GameOfLifeOpenCL : public GameOfLifeBase
{
public:
	GameOfLifeOpenCL(Life* life, int sizeX, int sizeY);
	virtual ~GameOfLifeOpenCL();

	const Life* Simulate(int generations) override;

	bool Init(cl::Device device);

protected:
	void Simulate();

	void SwapBuffers();

	static constexpr const char* const KernelFilePath = "kernel.cl";

	cl::CommandQueue m_queue;
	cl::Kernel m_kernel;

	cl::Buffer* m_clLife;
	cl::Buffer* m_clBuffer;

	cl::Device m_device;

	cl::NDRange m_global;
	cl::NDRange m_local;
};
