#pragma once

#include "Common.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLTexture
	{
	public:
		GLTexture(const char* filePath, GLint format);
		~GLTexture();

		void Bind(GLuint slot = 0) const;
		void Unbind() const;

		static void Init();

	private:
		GLuint m_RendererID;
	};

}