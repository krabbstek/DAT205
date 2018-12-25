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

}

#pragma warning(default: 4244)