#include "GameOfLife.h"
#include <cstring>
#include <iostream>

GameOfLife::GameOfLife(Life* life, int sizeX, int sizeY) : m_sizeY(sizeY), m_sizeX(sizeX), m_life(life), m_buffer(new Life[sizeY * sizeX])
{}

GameOfLife::~GameOfLife()
{
	delete[] m_life;
	delete[] m_buffer;
}

Life* GameOfLife::Simulate(int generations, char* mode)
{
	for (int g = 0; g < generations; g++)
	{
		Simulate(mode);
		SwapBuffers();
	}

	return m_life;
}

void GameOfLife::CheckField(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1) const
{
	int count = 0;
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

void GameOfLife::CheckLine(int y) const
{
	int ym1Offset, yOffset, yp1Offset, xm1, xp1;
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
		if (x == 0)
			xm1 = m_sizeX - 1;
		else
			xm1 = x - 1;

		if (x == m_sizeX - 1)
			xp1 = 0;
		else
			xp1 = x + 1;

		CheckField(ym1Offset, yOffset, yp1Offset, xm1, x, xp1);
	}
}

void GameOfLife::Simulate(char* mode) const
{
	if (strcmp(mode, "seq") == 0)
		for (int y = 0; y < m_sizeY; ++y)
		{
			CheckLine(y);
		}
	else if (strcmp(mode, "omp") == 0)
#pragma omp parallel for
		for (int y = 0; y < m_sizeY; ++y)
		{
			CheckLine(y);
		}
	else if (strcmp(mode, "omp") == 0)
		for (int y = 0; y < m_sizeY; ++y)
		{
			CheckLine(y);
		}
	else
		std::cout << "Error: mode not valid: " << mode << std::endl;
}

void GameOfLife::SwapBuffers()
{
	Life* tmp = m_life;
	m_life = m_buffer;
	m_buffer = tmp;
}
