#pragma once
#include "GameOfLife.h"

static class FileIO
{
public:
	static Life* Load(char* in_file, int& sizeX, int& sizeY);
	static void Save(char* out_file, Life* life, int sizeX, int sizeY);
};

