#include "pch.h"

#include "GLVertexArray.h"

#include "GLCommon.h"

namespace core {

	GLVertexArray::GLVertexArray(const GLVertexBuffer& vbo)
	{
		const GLVertexBufferLayout& layout = vbo.GetVertexBufferLayout();
		unsigned int i = 0;
		unsigned int offset = 0;
		GLCall(glGenVertexArrays(1, &m_RendererID));
		vbo.Bind();
		Bind();
		for (std::pair<GLuint, unsigned int> location : layout.GetLayoutElements())
		{
			GLCall(glVertexAttribPointer(i, location.second, location.first, /*normalized =*/ GL_FALSE, layout.GetStride(), /*pointer =*/ (const void*)offset));
			GLCall(glEnableVertexAttribArray(i++));
			offset += GLVertexBufferLayout::GetElementSize(location.first) * location.second;
		}
	}

	GLVertexArray::~GLVertexArray()
	{
		if (m_RendererID)
		{
			if (s_CurrentRendererID == m_RendererID)
			{
				GLCall(glBindVertexArray(0));
				s_CurrentRendererID = 0;
			}
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}
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