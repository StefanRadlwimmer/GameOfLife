#include "FileIO2.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

bool* FileIO2::Load(char* in_file, int& sizeX, int& sizeY)
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

	bool* life = new bool[sizeY * sizeX];

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

void FileIO2::Save(char* out_file, bool* life, int sizeX, int sizeY)
{
	std::ofstream outfile(out_file);
	outfile << sizeX << "," << sizeY << std::endl;
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; x++)
		{
			if (life[y *sizeX + x])
				outfile << 'x';
			else
				outfile << '.';
		}
		outfile << std::endl;
	}
}
