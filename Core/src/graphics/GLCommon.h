#pragma once

#include "Common.h"

#include <GL/glew.h>

namespace core {

	extern CORE_API void GLClearErrors();
	extern CORE_API GLenum GLGetError();
	extern CORE_API bool GLLogError(const char* call, const char* file, int line);

}

#ifdef DEBUG
	#define GLCall(x) core::GLClearErrors();\
		x;\
		if (!core::GLLogError(#x, __FILE__, __LINE__)) __debugbreak()
#else
	#define GLCall(x) x
#endif