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


	GLVertexBufferLayout::GLVertexBufferLayout()
		: m_Stride(0)
	{
	}


	void GLVertexBufferLayout::Push(GLuint type, unsigned int count)
	{
		m_Layout.push_back(LayoutElement(type, count));
		m_Stride += count * GetElementSize(type);
	}


	unsigned int GLVertexBufferLayout::GetElementSize(GLuint type)
	{
		switch (type)
		{
		case GL_UNSIGNED_BYTE:
			return 1;

		case GL_FLOAT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			return 4;

		default:
			return 0;
		}
	}

}