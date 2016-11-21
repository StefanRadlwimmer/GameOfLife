#pragma once
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.h"
#include <CL/cl.hpp>

enum DeviceType
{
	CPU,
	GPU
};

enum Mode
{
	Seq,
	OpenMP,
	OpenCL
};

typedef int Life;

class Global
{
public:
	static std::string GetClErrorString(cl_int err);
	static void Global::CheckClError(cl_int err, const char *file, int line);
	static Mode ParseMode(char* mode);
	static DeviceType ParseDevice(char* device);
	static void DetermineBestWorkGroups(cl::Device device, int dim1, int dim2, cl::NDRange& local, cl::NDRange& global);
};