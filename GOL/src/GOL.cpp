// GOL.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>

#include "FileIO.h"
#include "OpenCLHelper.h"
#include "GameOfLife/GameOfLifeBase.h"
#include "GameOfLife/GameOfLifeSeq.h"
#include "GameOfLife/GameOfLifeOpenMP.h"
#include "GameOfLife/GameOfLifeOpenCL.h"

namespace cr = std::chrono;
typedef cr::high_resolution_clock Clock;

void WriteDuration(const Clock::time_point& start, const Clock::time_point& end);
void WriteTime(const Clock::time_point& start, const Clock::time_point& end);

GameOfLifeBase* InitLogic(const Arguments arg, Life* life, int sizeX, int sizeY, const Mode mode);

int main(int argc, char* argv[])
{
	Arguments arg = Arguments::Parse(argc, argv);

	if (arg.inFile == nullptr)
	{
		Arguments::PrintHelp();
		std::cin.ignore();
		return 1;
	}

	int sizeX, sizeY;
	const Clock::time_point start_FullRun = Clock::now();
	const Clock::time_point start_Init = Clock::now();

	Life* life = FileIO::Load(arg.inFile, sizeX, sizeY);
	if (life == nullptr)
		Global::PrintError("Input-File not found!", -1);
	if (sizeX == 0 && sizeY == 0)
		Global::PrintError("Input-File not valid!", -2);

	const Mode mode = Global::ParseMode(arg.modeString);
	GameOfLifeBase* gol = InitLogic(arg, life, sizeX, sizeY, mode);

	if (gol == nullptr)
		Global::PrintError("Input-File not valid!", -3);

	if (arg.measure)
		WriteDuration(start_Init, Clock::now());

	const Clock::time_point start_Simulate = Clock::now();
	const Life* result = gol->Simulate(arg.generations);
	if (arg.measure)
		WriteDuration(start_Simulate, Clock::now());

	const Clock::time_point start_DeInit = Clock::now();
	if (arg.outFile != nullptr)
		FileIO::Save(arg.outFile, result, sizeX, sizeY);

	const CompareResult compare = FileIO::Compare(arg.inFile, arg.compareFile);

	delete gol;
	if (arg.measure)
		WriteDuration(start_DeInit, Clock::now());
	if (arg.time)
		WriteTime(start_FullRun, Clock::now());

	switch (compare)
	{
	case CompareResult::Error: 
		std::cout << "\nError while comparing simulation output";
		break;
	case CompareResult::Unequal:
		std::cout << "\nSimulation wrong! Result differed!";
		break;
	case CompareResult::Equal:
		std::cout << "\nSimulation correct!";
		break;

	case CompareResult::Unknown: 
	default:
		break;
	}

	Global::WaitForRead();
	return 0;
}

GameOfLifeBase* InitLogic(const Arguments arg, Life* life, int sizeX, int sizeY, const Mode mode)
{
	switch (mode)
	{
	case Mode::Seq:
	{
		return new GameOfLifeSeq(life, sizeX, sizeY);
	}
	case Mode::OpenMP:
	{
		GameOfLifeOpenMP* golOpenMP = new GameOfLifeOpenMP(life, sizeX, sizeY);
		if (!golOpenMP->Init(arg.numTreads))
		{
			std::cerr << "Could not init OpenMP!";
			delete golOpenMP;
			return nullptr;
		}

		return golOpenMP;
	}
	case Mode::OpenCL:
	{
		GameOfLifeOpenCL* golOpenCL = new GameOfLifeOpenCL(life, sizeX, sizeY);

		const cl::Device device = (arg.platformId != -1 && arg.deviceId != -1)
			? OpenCLHelper::GetDevice(arg.platformId, arg.deviceId)
			: OpenCLHelper::GetDevice(OpenCLHelper::ParseDevice(arg.deviceString));

		if (!golOpenCL->Init(device))
		{
			std::cerr << "Could not init OpenCL!";
			delete golOpenCL;
			return nullptr;
		}

		return golOpenCL;
	}
	}
	return nullptr;
}

void WriteDuration(const Clock::time_point& start, const Clock::time_point& end)
{
	auto diff = end - start;

	auto hours = cr::duration_cast<cr::hours>(diff);
	diff -= hours;

	auto mins = cr::duration_cast<cr::minutes>(diff);
	diff -= mins;

	auto secs = cr::duration_cast<cr::seconds>(diff);
	diff -= secs;

	auto millis = cr::duration_cast<cr::milliseconds>(diff);

	std::cout << std::setfill('0');
	std::cout << std::setw(2) << hours.count() << ':'
		<< std::setw(2) << mins.count() << ':'
		<< std::setw(2) << secs.count() << '.'
		<< std::setw(3) << millis.count() << "; ";
}

void WriteTime(const Clock::time_point& start, const Clock::time_point& end)
{
	auto diff = end - start;
	auto secs = cr::duration_cast<cr::seconds>(diff);
	diff -= secs;

	auto millis = cr::duration_cast<cr::milliseconds>(diff);

	std::cout << std::setfill('0');
	std::cout << std::setw(2) << secs.count() << '.'
		<< std::setw(3) << millis.count() << "; " << std::endl;
}
