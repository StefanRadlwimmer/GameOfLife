#pragma once
#include "GameOfLifeBase.h"

class GameOfLifeSeq : public GameOfLifeBase
{
public:
	GameOfLifeSeq(Life* life, int sizeX, int sizeY);
	virtual ~GameOfLifeSeq();

	const Life* Simulate(int generations) override;

protected:
	virtual void Simulate();

	void SimulateLine(int ym1, int y, int yp1);
	void SimulateCell(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1);
	void SwapBuffers();

	Life* m_buffer;
};
