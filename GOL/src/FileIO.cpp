#include "FileIO.h"
#include <fstream>
#include <sstream>

Life* FileIO::Load(const char* in_file, int& sizeX, int& sizeY)
{
	std::ifstream infile(in_file);

	if (!infile.good())
		return nullptr;

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

		const int yOffset = y * sizeX;
		for (int x = 0; x < sizeX; ++x)
		{
			life[yOffset + x] = (cline[x] == 'x') ? 1 : 0;
		}
	}
	infile.close();

	return life;
}

void FileIO::Save(const char* out_file, const Life* life, int sizeX, int sizeY)
{
	std::ofstream outfile(out_file);
	outfile << sizeX << "," << sizeY << std::endl;
	char* outBuffer = new char[sizeX];
	for (int y = 0; y < sizeY; ++y)
	{
		const int yOffset = y * sizeX;
		for (int x = 0; x < sizeX; ++x)
		{
			outBuffer[x] = (life[yOffset + x] == 1) ? 'x' : '.';
		}
		outfile.write(outBuffer, sizeX);
		outfile << std::endl;
	}
	delete[] outBuffer;
}

CompareResult FileIO::Compare(const char* file1, const char* file2)
{
	if (file1 == nullptr || file2 == nullptr)
		return CompareResult::Unknown;

	int sizeX1, sizeY1, sizeX2, sizeY2;

	Life* life1 = Load(file1, sizeX1, sizeY1);
	Life* life2 = Load(file2, sizeX2, sizeY2);

	if (life1 == nullptr || life2 == nullptr || sizeX1 != sizeX2 || sizeY1 != sizeY2)
	{
		delete life1;
		delete life2;
		return CompareResult::Error;
	}

	const CompareResult result = (memcmp(life1, life2, sizeX1 * sizeY1 * sizeof(Life)) != 0) ? CompareResult::Equal : CompareResult::Unequal;

	delete life1;
	delete life2;
	return result;
}
