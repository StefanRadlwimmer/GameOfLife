#include "GameOfLifeSeq.h"
#include <algorithm>

GameOfLifeSeq::GameOfLifeSeq(Life* life, int sizeX, int sizeY)
	: GameOfLifeBase(life, sizeX, sizeY)
	, m_buffer(new Life[sizeY * sizeX])
{}

GameOfLifeSeq::~GameOfLifeSeq()
{
	delete[] m_buffer;
}

const Life* GameOfLifeSeq::Simulate(int generations)
{
	for (int g = 0; g < generations; ++g)
	{
		Simulate();

		SwapBuffers();
	}

	return m_life;
}

void GameOfLifeSeq::Simulate()
{
	const int lastLine = m_sizeY - 1;

	// Top wrap-around
	SimulateLine(lastLine, 0, 1);

	for (int y = 1; y < lastLine; ++y)
	{
		SimulateLine(y - 1, y, y + 1);
	}

	// Bottom wrap-around
	SimulateLine(lastLine - 1, lastLine, 0);
}

void GameOfLifeSeq::SimulateLine(int ym1, int y, int yp1)
{
	const int ym1Offset = ym1 * m_sizeX;
	const int yOffset = y * m_sizeX;
	const int yp1Offset = yp1 * m_sizeX;

	const int lastField = m_sizeX - 1;

	// Left wrap-around
	SimulateCell(ym1Offset, yOffset, yp1Offset, lastField, 0, 1);

	for (int x = 1; x < lastField; ++x)
	{
		SimulateCell(ym1Offset, yOffset, yp1Offset, x - 1, x, x + 1);
	}

	// Right wrap-around
	SimulateCell(ym1Offset, yOffset, yp1Offset, lastField - 1, lastField, 0);
}

void GameOfLifeSeq::SimulateCell(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1)
{
	const int count = 
		  m_life[ym1Offset + xm1] + m_life[ym1Offset + x] + m_life[ym1Offset + xp1]
		+ m_life[yOffset   + xm1]						  + m_life[yOffset   + xp1]
		+ m_life[yp1Offset + xm1] + m_life[yp1Offset + x] + m_life[yp1Offset + xp1];

	m_buffer[yOffset + x] = ((count == 2 && m_life[yOffset + x]) || count == 3) ? 1 : 0;
}

void GameOfLifeSeq::SwapBuffers()
{
	std::swap(m_life, m_buffer);
}
