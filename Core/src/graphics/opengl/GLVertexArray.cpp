#include "pch.h"

#include "GLVertexArray.h"

#include "GLCommon.h"

namespace core {

	GLVertexArray::GLVertexArray()
		: m_NumLocations(0)
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
	}

	GLVertexArray::~GLVertexArray()
	{
		if (m_RendererID)
		{
			GLCall(glDeleteVertexArrays(1, &m_RendererID));
		}
	}


	void GLVertexArray::AddVertexBuffer(const GLVertexBuffer& vbo)
	{
		const GLVertexBufferLayout& layout = vbo.GetVertexBufferLayout();
		unsigned int offset = 0;
		
		vbo.Bind();
		Bind();
		for (std::pair<GLuint, unsigned int> location : layout.GetLayoutElements())
		{
			GLCall(glVertexAttribPointer(m_NumLocations, location.second, location.first, /*normalized =*/ GL_FALSE, layout.GetStride(), /*pointer =*/ (const void*)offset));
			GLCall(glEnableVertexAttribArray(m_NumLocations++));
			offset += GLVertexBufferLayout::GetElementSize(location.first) * location.second;
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