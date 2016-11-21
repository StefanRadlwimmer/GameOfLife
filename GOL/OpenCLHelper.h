#pragma once
#include "Global.h"

class OpenCLHelper
{
public:
	static void GetOpenCLSettings(DeviceType device, int& platformId, int& deviceID);
	static std::string GetClErrorString(cl_int err);
	static void CheckClError(cl_int err, const char *file, int line);
	static void DetermineBestWorkGroups(cl::Device device, int dim1, int dim2, cl::NDRange& local, cl::NDRange& global);
	static cl::Device GetDevice(int platformId, int deviceId);
};
