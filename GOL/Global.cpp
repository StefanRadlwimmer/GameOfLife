#include "Global.h"
#include <iostream>

Mode Global::ParseMode(char* mode)
{
	if (strcmp(mode, "ocl") == 0)
		return OpenCL;
	if (strcmp(mode, "omp") == 0)
		return OpenMP;
	if (strcmp(mode, "seq") == 0)
		return Seq;

	std::cout << "Unknown Mode: " << mode << std::endl;
	return Seq;
}

DeviceType Global::ParseDevice(char* device)
{
	if (strcmp(device, "cpu") == 0)
		return CPU;
	if (strcmp(device, "gpu") == 0)
		return GPU;

	std::cout << "Unknown Device: " << device << std::endl;
	return GPU;
}