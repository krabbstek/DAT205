#include "GLVertexBuffer.h"

#include "GLCommon.h"

GLVertexBuffer::GLVertexBuffer(const void* data, unsigned int size)
{
	GLCall(glGenBuffers(1, &m_RendererID));
	SetData(data, size);
}

GLVertexBuffer::~GLVertexBuffer()
{
	if (m_RendererID)
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}
}


void GLVertexBuffer::SetData(const void* data, unsigned int size) const
{
	Bind();
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}


void GLVertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void GLVertexBuffer::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}