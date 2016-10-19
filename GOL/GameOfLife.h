#pragma once

typedef bool Life;

class GameOfLife
{
public:
	GameOfLife(Life* life, int sizeX, int sizeY);
	~GameOfLife();

	Life* Simulate(int generations, char* mode);
	void CheckField(int ym1Offset, int yOffset, int yp1Offset, int xm1, int x, int xp1) const;
	void CheckLine(int y) const;
private:
	void Simulate(char* mode) const;
	void SwapBuffers();

	int m_sizeY;
	int m_sizeX;
	Life* m_life;
	Life* m_buffer;
};

