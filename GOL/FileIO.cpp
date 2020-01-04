#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

Life* FileIO::Load(char* in_file, int& sizeX, int& sizeY)
{
	std::ifstream infile(in_file);

	std::string line, item;
	std::getline(infile, line);
	std::stringstream ss;
	ss.str(line);
	std::getline(ss, item, ',');
	sizeX = atoi(item.c_str());
	std::getline(ss, item, ',');
	sizeY = atoi(item.c_str());

	Life* life = new Life[sizeY * sizeX];

	for (int y = 0; y < sizeY; ++y)
	{
		std::getline(infile, line);
		const char* cline = line.c_str();
		for (int x = 0; x < sizeX; x++)
		{
			life[y *sizeX + x] = (cline[x] == 'x');
		}
	}
	infile.close();

	return life;
}

void FileIO::Save(char* out_file, Life* life, int sizeX, int sizeY)
{
	std::ofstream outfile(out_file);
	outfile << sizeX << "," << sizeY << std::endl;
	char* outBuffer = new char[sizeX];
	for (int y = 0; y < sizeY; ++y)
	{
		int yOffset = y * sizeX;
		for (int x = 0; x < sizeX; ++x)
		{
			if (life[yOffset + x])
				outBuffer[x] = 'x';
			else
				outBuffer[x] = '.';
		}
		outfile.write(outBuffer, sizeX);
		outfile << std::endl;
	}
	delete[] outBuffer;
}
