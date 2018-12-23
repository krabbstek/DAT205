#include "pch.h"

#include "GLCommon.h"

namespace core {

	void CORE_API GLClearErrors()
	{
		while (glGetError());
	}

	GLenum CORE_API GLGetError()
	{
		return glGetError();
	}

	bool CORE_API GLLogError(const char* call, const char* file, int line)
	{
		GLenum error = GLGetError();
		if (error != GL_NO_ERROR)
		{
			CORE_ERROR("OpenGL: function %s, file %s, line %d", call, file, line);
			return false;
		}
		return true;
	}

}