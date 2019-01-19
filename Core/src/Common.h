#pragma once

#ifdef CORE_BUILD_DLL
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#include "system/Memory.h"

#define BIT(x) (1 << x)