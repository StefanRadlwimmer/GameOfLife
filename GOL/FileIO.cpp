#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool** FileIO::Load(char* in_file, int& sizeX, int& sizeY)
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

	bool** life = new bool*[sizeY];

	for (int y = 0; y < sizeY; ++y)
	{
		life[y] = new bool[sizeX];
		std::getline(infile, line);
		const char* cline = line.c_str();
		for (int x = 0; x < sizeX; x++)
		{
			life[y][x] = (cline[x] == 'x');
		}
	}
	infile.close();

	return life;
}

void FileIO::Save(char* out_file, bool** life, int sizeX, int sizeY)
{
	std::ofstream outfile(out_file);
	outfile << sizeX << "," << sizeY << std::endl;
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (life[y][x])
				outfile << 'x';
			else
				outfile << '.';
		}
		outfile << std::endl;
	}
}
