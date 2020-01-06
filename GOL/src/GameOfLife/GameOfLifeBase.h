#pragma once
#include "Global.h"

class GameOfLifeBase
{
public:
	GameOfLifeBase(Life* life, int sizeX, int sizeY);
	virtual ~GameOfLifeBase();

	virtual const Life* Simulate(int generations) = 0;

protected:
	int m_sizeY;
	int m_sizeX;
	Life* m_life;
};
