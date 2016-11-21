#pragma once

#include "Global.h"
#include <CL/cl.hpp>

class GameOfLife
{
public:
	GameOfLife(Life* life, int sizeX, int sizeY);
	~GameOfLife();

	Life* Simulate(int generations, Mode mode);
	void SetCLSettings(int platformId, int deviceId);
	void SetupOpenCL();
private:
	void CheckLine(int ym1, int y, int yp1) const;
	void CheckField(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1) const;

	void SimulateSeq(int generations);
	void SimulateOpenMP(int generations);
	void SimulateOpenCL(int generations);
	void SwapBuffers();
	void SwapCLBuffers();

	//Seq & OpenMP
	int m_sizeY;
	int m_sizeX;
	Life* m_life;
	Life* m_buffer;

	//OpenCL
	cl::CommandQueue m_queue;
	cl::Kernel m_kernel;
	cl::Buffer* m_clLife;
	cl::Buffer* m_clBuffer;
	cl::Device m_device;
	int m_platformId;
	int m_deviceId;
};

