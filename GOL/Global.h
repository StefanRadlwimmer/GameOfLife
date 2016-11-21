#pragma once
#ifdef _DEBUG
#define __CL_ENABLE_EXCEPTIONS
#endif

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

typedef char Life;

class Global
{
public:
	static Mode ParseMode(char* mode);
	static DeviceType ParseDevice(char* device);
	};