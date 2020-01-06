#pragma once
#include "GameOfLifeSeq.h"

class GameOfLifeOpenMP : public GameOfLifeSeq
{
public:
	GameOfLifeOpenMP(Life* life, int sizeX, int sizeY);
	virtual ~GameOfLifeOpenMP() = default;

	bool Init(int threadCount);

protected:
	void Simulate() override;
};
