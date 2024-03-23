#include "math_util.h"

float CubicInterpolate(float t)
{
	float t2 = t * t;
	return 3 * t2 - 2 * t * t2;
}