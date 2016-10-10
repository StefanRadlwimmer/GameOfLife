#pragma once
class GameOfLife2
{
public:
	GameOfLife2(bool* life, int sizeX, int sizeY);
	~GameOfLife2();

	bool* Simulate(int generations);
private:
	void Simulate();
	void SwapBuffers();

	int m_sizeY;
	int m_sizeX;
	bool* m_life;
	bool* m_buffer;
};

