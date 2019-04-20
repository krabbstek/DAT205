#include "GLIndexBuffer.h"

#include "GLCommon.h"

GLIndexBuffer::GLIndexBuffer(const unsigned int* data, unsigned int count)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	SetData(data, count);
}

GLIndexBuffer::~GLIndexBuffer()
{
	if (m_RendererID)
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}
}


void GLIndexBuffer::SetData(const unsigned int* data, unsigned int count)
{
	Bind();
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	m_Count = count;
}


void GLIndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void GLIndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}