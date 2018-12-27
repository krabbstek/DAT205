#include "pch.h"

#include "GLVertexBuffer.h"

#include "GLCommon.h"

namespace core {

	GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
	}

	GLVertexBuffer::~GLVertexBuffer()
	{
		if (m_RendererID)
		{
			if (s_CurrentRendererID == m_RendererID)
			{
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
				s_CurrentRendererID = 0;
			}
			GLCall(glDeleteBuffers(1, &m_RendererID));
		}
	}


	void GLVertexBuffer::Bind() const
	{
		if (s_CurrentRendererID != m_RendererID)
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
			s_CurrentRendererID = m_RendererID;
		}
	}

	void GLVertexBuffer::Unbind() const
	{
		if (s_CurrentRendererID)
		{
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
			s_CurrentRendererID = 0;
		}
	}


	GLuint GLVertexBuffer::s_CurrentRendererID = 0;

}