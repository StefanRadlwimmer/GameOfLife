#pragma once
#ifdef _DEBUG
#define __CL_ENABLE_EXCEPTIONS
#endif
#include <CL/cl.hpp>

namespace cl
{
	enum DeviceType : cl_device_type
	{
		None = CL_DEVICE_TYPE_DEFAULT,
		CPU = CL_DEVICE_TYPE_CPU,
		GPU = CL_DEVICE_TYPE_GPU,
		All = CL_DEVICE_TYPE_ALL,
	};
}


class OpenCLHelper
{
public:
	static cl::DeviceType ParseDevice(char* device);
	static char* ParseDevice(cl::DeviceType device);

	static cl::Device GetDevice(cl::DeviceType device);
	static cl::Device GetDevice(int platformId, int deviceId);

	static void DetermineBestWorkGroups(cl::Device device, int dim1, int dim2, cl::NDRange& global, cl::NDRange& local);

	static std::string GetClErrorString(cl_int err);
	static void CheckClError(cl_int err, const char *file, int line);
};
