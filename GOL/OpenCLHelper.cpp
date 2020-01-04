#include "OpenCLHelper.h"
#include <iostream>
#include <CL/cl.hpp>
#include <algorithm>

void OpenCLHelper::GetOpenCLSettings(DeviceType deviceType, int& out_platformId, int& out_deviceId)
{
	cl_int err;

	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	CheckClError(err, __FILE__, __LINE__);

	for (out_platformId = 0; out_platformId < platforms.size(); ++out_platformId)
	{
		cl::Platform& platform = platforms[out_platformId];

		std::vector<cl::Device> devices;
		err = platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
		CheckClError(err, __FILE__, __LINE__);
		for (out_deviceId = 0; out_deviceId < devices.size(); ++out_deviceId)
		{
			cl::Device& device = devices[out_deviceId];

			cl_device_type type = 0;
			err = device.getInfo(CL_DEVICE_TYPE, &type);
			CheckClError(err, __FILE__, __LINE__);

			if ((type == CL_DEVICE_TYPE_GPU && deviceType == GPU)
				|| (type == CL_DEVICE_TYPE_CPU && deviceType == CPU))
			{
				return;
			}
		}
	}

	std::cout << "No device for type: " << Global::ParseDevice(deviceType) << " available!" << std::endl;
}

void OpenCLHelper::DetermineBestWorkGroups(cl::Device device, int dim1, int dim2, cl::NDRange& out_local, cl::NDRange& out_global)
{
	dim2 = pow(2, ceil(log(dim2) / log(2)));

	out_global = cl::NDRange(dim1, dim2);

	cl_int err = CL_SUCCESS;
	size_t maxWorkGroupSize = 0;
	err = device.getInfo(CL_DEVICE_MAX_WORK_GROUP_SIZE, &maxWorkGroupSize);
	CheckClError(err, __FILE__, __LINE__);

	//I have no idea why, but this is faster
	size_t magicFactor = 4ULL;
	out_local = cl::NDRange(1, std::min(dim2 * 1ULL, maxWorkGroupSize / magicFactor));
}

cl::Device OpenCLHelper::GetDevice(int platformId, int deviceId)
{
	cl_int err = CL_SUCCESS;

	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	CheckClError(err, __FILE__, __LINE__);
	if (platforms.size() == 0)
	{
		std::cout << "No OpenCL platforms available!\n";
		exit(-1);
	}

	if (platforms.size() <= platformId)
	{
		std::cout << "No OpenCL platform with Id " << platformId << " available!\n";
		exit(-1);
	}

	cl::Platform platform = platforms[platformId];

	std::vector<cl::Device> devices;
	err = platform.getDevices(CL_DEVICE_TYPE_ALL, &devices);
	CheckClError(err, __FILE__, __LINE__);
	if (devices.size() == 0)
	{
		std::cout << "No OpenCL devices on platorm " << platformId << " available!\n";
		exit(-1);
	}

	if (devices.size() <= deviceId)
	{
		std::cout << "No OpenCL device with Id " << deviceId << " on platform " << platformId << " available!\n";
		exit(-1);
	}

	return devices[deviceId];
}

std::string OpenCLHelper::GetClErrorString(cl_int err)
{
	switch (err) {
	case CL_SUCCESS:                          return std::string("Success");
	case CL_DEVICE_NOT_FOUND:                 return std::string("Device not found");
	case CL_DEVICE_NOT_AVAILABLE:             return std::string("Device not available");
	case CL_COMPILER_NOT_AVAILABLE:           return std::string("Compiler not available");
	case CL_MEM_OBJECT_ALLOCATION_FAILURE:    return std::string("Memory object allocation failure");
	case CL_OUT_OF_RESOURCES:                 return std::string("Out of resources");
	case CL_OUT_OF_HOST_MEMORY:               return std::string("Out of host memory");
	case CL_PROFILING_INFO_NOT_AVAILABLE:     return std::string("Profiling information not available");
	case CL_MEM_COPY_OVERLAP:                 return std::string("Memory copy overlap");
	case CL_IMAGE_FORMAT_MISMATCH:            return std::string("Image format mismatch");
	case CL_IMAGE_FORMAT_NOT_SUPPORTED:       return std::string("Image format not supported");
	case CL_BUILD_PROGRAM_FAILURE:            return std::string("Program build failure");
	case CL_MAP_FAILURE:                      return std::string("Map failure");
	case CL_MISALIGNED_SUB_BUFFER_OFFSET:     return std::string("Misaligned sub buffer offset");
	case CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST: return std::string("Exec status error for events in wait list");
	case CL_INVALID_VALUE:                    return std::string("Invalid value");
	case CL_INVALID_DEVICE_TYPE:              return std::string("Invalid device type");
	case CL_INVALID_PLATFORM:                 return std::string("Invalid platform");
	case CL_INVALID_DEVICE:                   return std::string("Invalid device");
	case CL_INVALID_CONTEXT:                  return std::string("Invalid context");
	case CL_INVALID_QUEUE_PROPERTIES:         return std::string("Invalid queue properties");
	case CL_INVALID_COMMAND_QUEUE:            return std::string("Invalid command queue");
	case CL_INVALID_HOST_PTR:                 return std::string("Invalid host pointer");
	case CL_INVALID_MEM_OBJECT:               return std::string("Invalid memory object");
	case CL_INVALID_IMAGE_FORMAT_DESCRIPTOR:  return std::string("Invalid image format descriptor");
	case CL_INVALID_IMAGE_SIZE:               return std::string("Invalid image size");
	case CL_INVALID_SAMPLER:                  return std::string("Invalid sampler");
	case CL_INVALID_BINARY:                   return std::string("Invalid binary");
	case CL_INVALID_BUILD_OPTIONS:            return std::string("Invalid build options");
	case CL_INVALID_PROGRAM:                  return std::string("Invalid program");
	case CL_INVALID_PROGRAM_EXECUTABLE:       return std::string("Invalid program executable");
	case CL_INVALID_KERNEL_NAME:              return std::string("Invalid kernel name");
	case CL_INVALID_KERNEL_DEFINITION:        return std::string("Invalid kernel definition");
	case CL_INVALID_KERNEL:                   return std::string("Invalid kernel");
	case CL_INVALID_ARG_INDEX:                return std::string("Invalid argument index");
	case CL_INVALID_ARG_VALUE:                return std::string("Invalid argument value");
	case CL_INVALID_ARG_SIZE:                 return std::string("Invalid argument size");
	case CL_INVALID_KERNEL_ARGS:              return std::string("Invalid kernel arguments");
	case CL_INVALID_WORK_DIMENSION:           return std::string("Invalid work dimension");
	case CL_INVALID_WORK_GROUP_SIZE:          return std::string("Invalid work group size");
	case CL_INVALID_WORK_ITEM_SIZE:           return std::string("Invalid work item size");
	case CL_INVALID_GLOBAL_OFFSET:            return std::string("Invalid global offset");
	case CL_INVALID_EVENT_WAIT_LIST:          return std::string("Invalid event wait list");
	case CL_INVALID_EVENT:                    return std::string("Invalid event");
	case CL_INVALID_OPERATION:                return std::string("Invalid operation");
	case CL_INVALID_GL_OBJECT:                return std::string("Invalid OpenGL object");
	case CL_INVALID_BUFFER_SIZE:              return std::string("Invalid buffer size");
	case CL_INVALID_MIP_LEVEL:                return std::string("Invalid mip-map level");
	case CL_INVALID_GLOBAL_WORK_SIZE:         return std::string("Invalid gloal work size");
	case CL_INVALID_PROPERTY:                 return std::string("Invalid property");
	default:                                  return std::string("Unknown error code");
	}
}

void OpenCLHelper::CheckClError(cl_int err, const char* file, int line)
{
#ifdef _DEBUG
	if (err != CL_SUCCESS) {
		std::cerr << "OpenCL Error: " << GetClErrorString(err) << " | " << file << " (" << line << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
#endif
}

