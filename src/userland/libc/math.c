#include "math.h"

long pow(long num, long power)
{
	int invert = 0;

	long powMult = 1;

	if ((invert = (power < 0)))
		power = -power;

	for (long x = 0; x < power; x++)
		powMult *= num;

	return (invert) ? 1 / powMult : powMult;
}
