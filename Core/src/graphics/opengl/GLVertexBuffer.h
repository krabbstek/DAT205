#pragma once

#include "Common.h"

#include "GLVertexBufferLayout.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLVertexBuffer
	{
	public:
		GLVertexBuffer(const void* data, unsigned int size);
		~GLVertexBuffer();

		inline void SetVertexBufferLayout(const GLVertexBufferLayout& layout) { m_Layout = layout; }
		inline const GLVertexBufferLayout& GetVertexBufferLayout() const { return m_Layout; }

		void Bind() const;
		void Unbind() const;

	private:
		GLuint m_RendererID;
		GLVertexBufferLayout m_Layout;

		static GLuint s_CurrentRendererID;
	};

}