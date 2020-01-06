#pragma once
#include <cstdint>

enum class Mode
{
	Seq,
	OpenMP,
	OpenCL
};

typedef uint8_t Life;

enum class CompareResult
{
	Unknown,
	Error,
	Unequal,
	Equal,
};

class Global
{
public:
	static Mode ParseMode(const char* mode);
	static void WaitForRead();
	static void PrintError(const char* message, int code);
};

struct Arguments
{
	static Arguments Parse(int argc, char* argv[]);

	static void PrintHelp();

	char* inFile = nullptr;
	char* outFile = nullptr;
	char* compareFile = nullptr;
	char* modeString = "seq";
	char* deviceString = "gpu";

	int generations = 0;
	int numTreads = -1;
	int platformId = -1;
	int deviceId = -1;

	bool measure = false;
	bool time = false;
};