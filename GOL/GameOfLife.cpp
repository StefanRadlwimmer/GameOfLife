#include "GameOfLife.h"
#include "Modulo.h"

GameOfLife::GameOfLife(bool** life, int sizeX, int sizeY) : m_sizeY(sizeY), m_sizeX(sizeX), m_life(life)
{
	m_counter = new byte*[m_sizeY];
	for (int y = 0; y < m_sizeY; ++y)
		m_counter[y] = new byte[m_sizeX];
}

GameOfLife::~GameOfLife()
{
	for (int y = 0; y < m_sizeY; y++)
	{
		delete[] m_life[y];
	}
	delete[] m_life;
}

bool** GameOfLife::Simulate(int generations)
{
	for (int i = 0; i < generations; i++)
	{
		Simulate();
	}

	return m_life;
}

void GameOfLife::Simulate()
{
	for (int y = 0; y < m_sizeY; ++y)
		for (int x = 0; x < m_sizeX; ++x)
		{
			Count(y, x);
		}
	for (int y = 0; y < m_sizeY; ++y)
		for (int x = 0; x < m_sizeX; ++x)
		{
			CheckLife(y, x);
		}
}

void GameOfLife::Count(int y, int x)
{
	byte count = 0;
	if (m_life[mod(y - 1, m_sizeY)][mod(x - 1, m_sizeX)])
		++count;

	if (m_life[mod(y - 1, m_sizeY)][x])
		++count;

	if (m_life[mod(y - 1, m_sizeY)][mod(x + 1, m_sizeX)])
		++count;

	if (m_life[y][mod(x - 1, m_sizeX)])
		++count;

	if (m_life[y][mod(x + 1, m_sizeX)])
		++count;

	if (m_life[mod(y + 1, m_sizeY)][mod(x - 1, m_sizeX)])
		++count;

	if (m_life[mod(y + 1, m_sizeY)][x])
		++count;

	if (m_life[mod(y + 1, m_sizeY)][mod(x + 1, m_sizeX)])
		++count;

	m_counter[y][x] = count;
}

void GameOfLife::CheckLife(int y, int x)
{
	byte count = m_counter[y][x];

	m_life[y][x] = (count == 2 && m_life[y][x]) || count == 3;
}
