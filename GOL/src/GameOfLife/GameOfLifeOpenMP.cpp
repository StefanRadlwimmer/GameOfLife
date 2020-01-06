#include "GameOfLifeOpenMP.h"
#include <omp.h>

GameOfLifeOpenMP::GameOfLifeOpenMP(Life* life, int sizeX, int sizeY)
	: GameOfLifeSeq(life, sizeX, sizeY)
{}

bool GameOfLifeOpenMP::Init(int threadCount)
{
	if (threadCount != -1)
		omp_set_num_threads(threadCount);

	return true;
}

void GameOfLifeOpenMP::Simulate()
{
	const int lastLine = m_sizeY - 1;
#pragma omp parallel 
	{
#pragma omp single nowait
		// Top wrap-around
		SimulateLine(lastLine, 0, 1);

# pragma omp for nowait
		for (int y = 1; y < lastLine; ++y)
		{
			SimulateLine(y - 1, y, y + 1);
		}

#pragma omp single nowait
		// Bottom wrap-around
		SimulateLine(lastLine - 1, lastLine, 0);
	}
}
