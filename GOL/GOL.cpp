// GOL.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>
#include "FileIO.h"
#include "GameOfLife.h"

namespace cr = std::chrono;
typedef cr::high_resolution_clock golClock;
void WriteDuration(const cr::steady_clock::time_point& start, const cr::steady_clock::time_point& end);

int main(int argc, char* argv[])
{
	char* inFile = nullptr, *outFile = nullptr, *mode = "seq";
	int generations = 0;
	bool measure = false;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--load") == 0)
			inFile = argv[++i];

		if (strcmp(argv[i], "--generations") == 0)
			generations = atoi(argv[++i]);

		if (strcmp(argv[i], "--save") == 0)
			outFile = argv[++i];

		if (strcmp(argv[i], "--measure") == 0)
			measure = true;

		if (strcmp(argv[i], "--mode") == 0)
			mode = argv[++i];
	}

	if (inFile == nullptr)
	{
		std::cout << "Usage:" << std::endl <<
			"--load NAME (where NAME is a filename with the extension '.gol')" << std::endl <<
			"--generations NUM (where NUM is a number)." << std::endl <<
			"--save NAME (where NAME is a filename with the extension '.gol')" << std::endl <<
			"--measure (generates measurement output on stdout)" << std::endl <<
			"--mode [seq|omp|ocl] (Mode of computation)";
		return -1;
	}

	int sizeX, sizeY;
	auto start_time = golClock::now();
	auto life = FileIO::Load(inFile, sizeX, sizeY);
	auto gol = new GameOfLife(life, sizeX, sizeY);
	if (measure)
		WriteDuration(start_time, golClock::now());

	start_time = golClock::now();
	life = gol->Simulate(generations, mode);
	if (measure)
		WriteDuration(start_time, golClock::now());

	start_time = golClock::now();
	if (outFile != nullptr)
		FileIO::Save(outFile, life, sizeX, sizeY);
	delete gol;
	if (measure)
		WriteDuration(start_time, golClock::now());

#ifdef _DEBUG
	std::cin.ignore();
#endif
}

void WriteDuration(const cr::steady_clock::time_point& start, const cr::steady_clock::time_point& end)
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
