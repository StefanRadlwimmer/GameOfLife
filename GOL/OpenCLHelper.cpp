#include "OpenCLHelper.h"
#include <cstring>
#include <iostream>
#include <CL/cl.hpp>

void OpenCLHelper::GetOpenCLSettings(DeviceType device, int& out_platformId, int& out_deviceId)
{
	cl_int err;

	cl_uint numPlatforms;
	cl_platform_id* platformIds;
	cl_uint numDevices;
	cl_device_id* deviceIds;

	err = clGetPlatformIDs(0, nullptr, &numPlatforms);
	Global::CheckClError(err, __FILE__, __LINE__);
	platformIds = (cl_platform_id*)alloca(sizeof(cl_platform_id)*numPlatforms);
	err = clGetPlatformIDs(numPlatforms, platformIds, nullptr);
	Global::CheckClError(err, __FILE__, __LINE__);

	for (int p = 0; p < numPlatforms; ++p)
	{
		cl_platform_id platformId = platformIds[p];

		err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, NULL, &numDevices);
		Global::CheckClError(err, __FILE__, __LINE__);
		deviceIds = (cl_device_id*)alloca(sizeof(cl_platform_id)*numDevices);
		err = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, numDevices, deviceIds, NULL);
		Global::CheckClError(err, __FILE__, __LINE__);

		for (int d = 0; d < numDevices; ++d)
		{
			cl_device_id deviceId = deviceIds[d];

			cl_device_type type = 0;
			err = clGetDeviceInfo(deviceId, CL_DEVICE_TYPE, sizeof(cl_device_type), &type, NULL);
			Global::CheckClError(err, __FILE__, __LINE__);

			if ((type == CL_DEVICE_TYPE_GPU && device == GPU) 
				|| (type == CL_DEVICE_TYPE_CPU && device == CPU))
			{
				out_platformId = p;
				out_deviceId = d;
				return;
			}
		}
	}

	std::cout << "Unknown Device: " << device << std::endl;
	return;

	std::vector<cl::Platform> platforms;
	err = cl::Platform::get(&platforms);
	Global::CheckClError(err, __FILE__, __LINE__);

	for (std::vector<cl::Platform>::iterator it = platforms.begin(); it != platforms.end(); ++it)
	{
		std::vector<cl::Device> devices;
		if (device == CPU)
			err = (*it).getDevices(CL_DEVICE_TYPE_CPU, &devices);
		else
			err = (*it).getDevices(CL_DEVICE_TYPE_GPU, &devices);
		Global::CheckClError(err, __FILE__, __LINE__);

		if (devices.size() != 0)
		{
			cl_platform_id id = (*it)();
			/*platformId = (*it);
			deviceID = devices[0].getInfo()*/
		}
	}
}
