__kernel void SimulateCell(
	__global const char* life,
	__global char* buffer,
	const int sizeY,
	const int sizeX
)
{
	int y = get_global_id(0);
	int x = get_global_id(1);

	//Rounding up globalRange to 2^n --> need to check for overflow
	if (y >= sizeY || x >= sizeX)
		return;

	const int yOffset = y * sizeX;

	const int ym1Offset = (y == 0) ? (sizeY - 1) * sizeX : yOffset - sizeX;
	const int yp1Offset = (y == sizeY - 1) ? 0 : yOffset + sizeX;

	const int xm1 = (x == 0) ? sizeX - 1 : x - 1;
	const int xp1 = (x == sizeX - 1) ? 0 : x + 1;
	
	const int count =
		  life[ym1Offset + xm1] + life[ym1Offset + x] + life[ym1Offset + xp1]
		+ life[yOffset   + xm1]						  + life[yOffset   + xp1]
		+ life[yp1Offset + xm1] + life[yp1Offset + x] + life[yp1Offset + xp1];

	buffer[yOffset + x] = ((count == 2 && life[yOffset + x]) || count == 3) ? 1 : 0;
}