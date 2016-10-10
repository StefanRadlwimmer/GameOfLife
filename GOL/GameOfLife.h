#pragma once

typedef char byte;

class GameOfLife
{
public:
	GameOfLife(bool** life, int sizeX, int sizeY);
	~GameOfLife();

	bool** Simulate(int generations);

private:
	void Simulate();
	void Count(int y, int x);
	void CheckLife(int y, int x);
	
	int m_sizeY;
	int m_sizeX;
	bool** m_life;
	byte** m_counter;
};

