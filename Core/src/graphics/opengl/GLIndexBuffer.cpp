#include "pch.h"

#include "GLIndexBuffer.h"

#include "GLCommon.h"

namespace core {

	GLuint GLIndexBuffer::s_CurrentRendererID = 0;


	GLIndexBuffer::GLIndexBuffer(const unsigned int* data, unsigned int count)
		: m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		Bind();
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	GLIndexBuffer::~GLIndexBuffer()
	{
		if (m_RendererID)
		{
			if (s_CurrentRendererID == m_RendererID)
			{
				GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
				s_CurrentRendererID = 0;
			}
			GLCall(glDeleteBuffers(1, &m_RendererID));
		}
	}


	void GLIndexBuffer::Bind() const
	{
		if (s_CurrentRendererID != m_RendererID)
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
			s_CurrentRendererID = m_RendererID;
		}
	}

	void GLIndexBuffer::Unbind() const
	{
		if (s_CurrentRendererID)
		{
			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			s_CurrentRendererID = 0;
		}
	}

}