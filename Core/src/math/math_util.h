#pragma once

#include "Common.h"

#pragma warning(disable: 4244)

namespace core {

#define PI 3.141592653589793

	inline constexpr float CORE_API DegToRad(float degrees)
	{
		return degrees * (PI / 180.0);
	}

	inline constexpr float CORE_API RadToDeg(float radians)
	{
		return radians * (180.0 / PI);
	}


	constexpr float CORE_API Clamp(float val, float min, float max);


	inline constexpr float CORE_API LinearInterpolate(float t, float t0, float t1)
	{
		return t * t0 + (1.0f - t) * t1;
	}

	float CORE_API CubicInterpolate(float t);

}

#pragma warning(default: 4244)