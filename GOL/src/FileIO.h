#pragma once
#include "Global.h"

class FileIO
{
public:
	static Life* Load(const char* filePath, int& sizeX, int& sizeY);
	static void Save(const char* filePath, const Life* life, int sizeX, int sizeY);

	static CompareResult Compare(const char* file1, const char* file2);
};

