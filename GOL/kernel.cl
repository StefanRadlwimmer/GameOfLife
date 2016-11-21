__kernel void CheckLife(
	__global const int* life,
	__global int* buffer,
	const int sizeY,
	const int sizeX
)
{
	int y = get_global_id(0);
	int x = get_global_id(1);

	//Rounding up globalRange to 2^n --> need to check for overflow
	if (y >= sizeY || x >= sizeX)
		return;

	int ym1Offset, yOffset, yp1Offset, xm1, xp1;

	yOffset = y * sizeX;
	if (y == 0)
		ym1Offset = (sizeY - 1) * sizeX;
	else
		ym1Offset = yOffset - sizeX;

	if (y == sizeY - 1)
		yp1Offset = 0;
	else
		yp1Offset = yOffset + sizeX;

	if (x == 0)
		xm1 = sizeX - 1;
	else
		xm1 = x - 1;

	if (x == sizeX - 1)
		xp1 = 0;
	else
		xp1 = x + 1;

	int count = 0;
	if (life[ym1Offset + xm1])
		++count;

	if (life[ym1Offset + x])
		++count;

	if (life[ym1Offset + xp1])
		++count;

	if (life[yOffset + xm1])
		++count;

	if (life[yOffset + xp1])
		++count;

	if (life[yp1Offset + xm1])
		++count;

	if (life[yp1Offset + x])
		++count;

	if (life[yp1Offset + xp1])
		++count;

	buffer[yOffset + x] = (count == 2 && life[yOffset + x]) || count == 3;
}