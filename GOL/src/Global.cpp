#include "Global.h"
#include <iostream>

Mode Global::ParseMode(const char* mode)
{
	if (mode == nullptr)
		return Mode::Seq;

	if (strcmp(mode, "ocl") == 0)
		return Mode::OpenCL;
	if (strcmp(mode, "omp") == 0)
		return Mode::OpenMP;
	if (strcmp(mode, "seq") == 0)
		return Mode::Seq;

	std::cout << "Unknown Mode: " << mode << std::endl;
	return Mode::Seq;
}

void Global::WaitForRead()
{
#ifdef _DEBUG
	std::cin.ignore();
#endif
}

void Global::PrintError(const char* message, int code)
{
	std::cerr << message;
	WaitForRead();
	exit(code);
}

Arguments Arguments::Parse(int argc, char* argv[])
{
	Arguments arguments;

	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "--load") == 0)
			arguments.inFile = argv[++i];

		if (strcmp(argv[i], "--generations") == 0)
			arguments.generations = atoi(argv[++i]);

		if (strcmp(argv[i], "--save") == 0)
			arguments.outFile = argv[++i];

		if (strcmp(argv[i], "--compare") == 0)
			arguments.compareFile = argv[++i];

		if (strcmp(argv[i], "--measure") == 0)
			arguments.measure = true;

		if (strcmp(argv[i], "--time") == 0)
			arguments.time = true;

		if (strcmp(argv[i], "--mode") == 0)
			arguments.modeString = argv[++i];

		if (strcmp(argv[i], "--threads") == 0)
			arguments.numTreads = atoi(argv[++i]);

		if (strcmp(argv[i], "--device") == 0)
			arguments.deviceString = argv[++i];

		if (strcmp(argv[i], "--platformId") == 0)
			arguments.platformId = atoi(argv[++i]);

		if (strcmp(argv[i], "--deviceId") == 0)
			arguments.deviceId = atoi(argv[++i]);
	}

	return arguments;
}

void Arguments::PrintHelp()
{
	std::cout << "Usage:" << std::endl <<
		"--load NAME (where NAME is a filename with the extension '.gol')" << std::endl <<
		"--generations NUM (where NUM is a number)." << std::endl <<
		"--save NAME (where NAME is a filename with the extension '.gol')" << std::endl <<
		"--compare NAME (where NAME is a filename with the extension '.gol')" << std::endl <<
		"--measure (generates measurement output on stdout)" << std::endl <<
		"--time (generates timing output on stdout)" << std::endl <<
		"--mode [seq|omp|ocl] (Mode of computation)" << std::endl <<
		"//Options for --mode omp" << std::endl <<
		"--threads NUM (Sets max number of threads)" << std::endl <<
		"//Options for --mode ocl" << std::endl <<
		"--device [cpu|gpu] (automatically selects platform & device)" << std::endl <<
		"--platformId NUM (user-dened selection of platform)" << std::endl <<
		"--deviceId NUM (user-defined selection of device on platform)";
}
