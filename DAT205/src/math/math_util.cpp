#include "math_util.h"

#include "vec3.h"

#include <random>

float CubicInterpolate(float t)
{
	float t2 = t * t;
	return 3 * t2 - 2 * t * t2;
}


void ConcentricSampleDisk(float *dx, float *dy) {
	float r, theta;
	float u1 = RandF();
	float u2 = RandF();

	// Map uniform random numbers to [-1, 1]^2
	float sx = 2.0f * u1 - 1.0f;
	float sy = 2.0f * u2 - 1.0f;

	// Map square to (r, theta)
	// Handle degeneracy at the origin
	if (sx == 0.0 && sy == 0.0)
	{
		*dx = 0.0;
		*dy = 0.0;
		return;
	}

	if (sx >= -sy)
	{
		// Handle first region of disk
		if (sx > sy) {
			r = sx;
			if (sy > 0.0)
				theta = sy / r;
			else
				theta = 8.0f + sy / r;
		}
		// Handle second region of disk
		else
		{
			r = sy;
			theta = 2.0f - sx / r;
		}
	}
	else
	{
		// Handle third region of disk
		if (sx <= sy)
		{
			r = -sx;
			theta = 4.0f - sy / r;
		}
		// Handle fourth region of disk
		else
		{
			r = -sy;
			theta = 6.0f + sx / r;
		}
	}

	theta *= PI / 4.0f;
	*dx = r * cosf(theta);
	*dy = r * sinf(theta);
}

vec3 CosineSampleHemisphere()
{
	vec3 ret;
	ConcentricSampleDisk(&ret.x, &ret.y);
	ret.z = sqrt(std::max(0.0f, 1.0f - ret.x * ret.x - ret.y * ret.y));
	return ret;
}

float RandF()
{
	return float(rand()) / float(RAND_MAX);
}