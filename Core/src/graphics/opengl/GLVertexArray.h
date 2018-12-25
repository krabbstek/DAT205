#pragma once

#include "Common.h"

#include "GLVertexBuffer.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLVertexArray
	{
	public:
		GLVertexArray(const GLVertexBuffer& vbo, const GLVertexBufferLayout& layout);
		~GLVertexArray();

		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_RendererID;

		static GLuint s_CurrentRendererID;
	};

}