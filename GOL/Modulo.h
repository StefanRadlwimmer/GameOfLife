#pragma once

inline int mod(const int value, const int divisor)
{
	if (value >= 0 && value < divisor)
		return value;

	if (value < 0)
		return value + divisor;

	return value - divisor;
}