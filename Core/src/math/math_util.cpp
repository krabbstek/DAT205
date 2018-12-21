#include "pch.h"

#include "math_util.h"

namespace core {

	constexpr float CORE_API Clamp(float val, float min, float max)
	{
		if (val <= min)
			return min;
		else if (val >= max)
			return max;
		return val;
	}

}