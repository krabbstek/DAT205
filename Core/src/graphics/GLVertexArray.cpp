#include "pch.h"

#include "GLVertexArray.h"

#include "GLCommon.h"

namespace core {

	GLVertexArray::GLVertexArray(const GLVertexBuffer& vbo, const GLVertexBufferLayout& layout)
	{
		unsigned int i = 0;
		unsigned int stride = 0;
		GLCall(glGenVertexArrays(1, &m_RendererID));
		Bind();
		vbo.Bind();
		for (std::pair<GLuint, unsigned int> location : layout.m_Layout)
		{
			GLCall(glVertexAttribPointer(i++, GLVertexBufferLayout::GetElementSize(location.first), location.first, /*normalized =*/ GL_FALSE, stride, /*pointer =*/ 0));
			stride += GLVertexBufferLayout::GetElementSize(location.first) * location.second;
		}
	}

	GLVertexArray::~GLVertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}


	void GLVertexArray::Bind() const
	{
		if (s_CurrentRendererID != m_RendererID)
		{
			GLCall(glBindVertexArray(m_RendererID));
			s_CurrentRendererID = m_RendererID;
		}
	}

	void GLVertexArray::Unbind() const
	{
		if (s_CurrentRendererID)
		{
			GLCall(glBindVertexArray(0));
			s_CurrentRendererID = m_RendererID;
		}
	}


	GLuint GLVertexArray::s_CurrentRendererID = 0;

}