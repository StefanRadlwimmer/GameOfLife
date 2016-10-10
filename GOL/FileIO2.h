#pragma once
static class FileIO2
{
public:
	static bool* Load(char* in_file, int& sizeX, int& sizeY);
	static void Save(char* out_file, bool* life, int sizeX, int sizeY);
};

