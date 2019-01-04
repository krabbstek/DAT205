#pragma once

#include "Common.h"

#include "GLVertexBuffer.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLVertexArray
	{
	public:
		GLVertexArray();
		~GLVertexArray();

		void AddVertexBuffer(const GLVertexBuffer& vbo);

		void Bind() const;
		static void Unbind();

	private:
		GLuint m_RendererID;
		unsigned int m_NumLocations;
	};

}