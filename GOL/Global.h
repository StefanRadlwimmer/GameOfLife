#pragma once
#ifdef _DEBUG
#define __CL_ENABLE_EXCEPTIONS
#endif
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
	static Mode ParseMode(char* mode);
	static DeviceType ParseDevice(char* device);
	};