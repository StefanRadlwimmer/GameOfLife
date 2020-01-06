#include "GameOfLifeBase.h"

GameOfLifeBase::GameOfLifeBase(Life* life, int sizeX, int sizeY)
	: m_sizeY(sizeY)
	, m_sizeX(sizeX)
	, m_life(life)
{}

GameOfLifeBase::~GameOfLifeBase()
{
	delete[] m_life;
}
