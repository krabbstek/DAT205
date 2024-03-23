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
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}
	}


	void GLVertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void GLVertexArray::Unbind()
	{
		GLCall(glBindVertexArray(0));
	}

}