#include "GameOfLife2.h"

GameOfLife2::GameOfLife2(bool* life, int sizeX, int sizeY) : m_sizeY(sizeY), m_sizeX(sizeX), m_life(life), m_buffer(new bool[sizeY*sizeX])
{}

GameOfLife2::~GameOfLife2()
{
	delete[] m_life;
	delete[] m_buffer;
}

bool* GameOfLife2::Simulate(int generations)
{
	for (int g = 0; g < generations; g++)
	{
		Simulate();
		SwapBuffers();
	}

	return m_life;
}

void GameOfLife2::Simulate()
{
	int count, ym1Offset, yOffset, yp1Offset, xm1, xp1;

	for (int y = 0; y < m_sizeY; ++y)
	{
		if (y == 0)
			ym1Offset = (m_sizeY - 1) * m_sizeX;
		else
			ym1Offset = (y - 1) * m_sizeX;

		yOffset = y * m_sizeX;

		if (y == m_sizeY - 1)
			yp1Offset = 0;
		else
			yp1Offset = (y + 1)  * m_sizeX;

		for (int x = 0; x < m_sizeX; ++x)
		{
			count = 0;
			if (x == 0)
				xm1 = m_sizeX - 1;
			else
				xm1 = x - 1;

			if (x == m_sizeX - 1)
				xp1 = 0;
			else
				xp1 = x + 1;

			if (m_life[ym1Offset + xm1])
				++count;

			if (m_life[ym1Offset + x])
				++count;

			if (m_life[ym1Offset + xp1])
				++count;

			if (m_life[yOffset + xm1])
				++count;

			if (m_life[yOffset + xp1])
				++count;

			if (m_life[yp1Offset + xm1])
				++count;

			if (m_life[yp1Offset + x])
				++count;

			if (m_life[yp1Offset + xp1])
				++count;

			m_buffer[yOffset + x] = (count == 2 && m_life[yOffset + x]) || count == 3;
		}
	}
}

void GameOfLife2::SwapBuffers()
{
	bool* tmp = m_life;
	m_life = m_buffer;
	m_buffer = tmp;
}
