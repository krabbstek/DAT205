#include "GLTexture2D.h"

#include "GLCommon.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

GLTexture2D::GLTexture2D()
	: m_Width(0), m_Height(0)
{
	GLCall(glGenTextures(1, &m_RendererID));
}

GLTexture2D::~GLTexture2D()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

	
void GLTexture2D::Load(GLuint openGLFormat, const void* data, unsigned int width, unsigned int height, GLuint dataFormat, GLuint type)
{
	Load(openGLFormat, data, width, height, dataFormat, type, 0);
}

void GLTexture2D::Load(GLuint openGLFormat, const void* data, unsigned int width, unsigned int height, GLuint dataFormat, GLuint type, int level)
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, level, openGLFormat, width, height, 0, dataFormat, type, data));

	if (!level)
	{
		m_Width = width;
		m_Height = height;
	}
}

bool GLTexture2D::LoadFromFile(const char* filePath)
{
	return LoadFromFile(filePath, 0);
}

bool GLTexture2D::LoadFromFile(const char* filePath, int level)
{
	int width, height, comp;
	unsigned char* data = stbi_load(filePath, &width, &height, &comp, 0);
	if (!data)
	{
		std::printf("Failed to load texture from file!");
		return false;
	}

	GLuint format;
	switch (comp)
	{
	case 1:
		format = GL_RED;
		break;

	case 2:
		format = GL_RG;
		break;

	case 3:
		format = GL_RGB;
		break;

	case 4:
		format = GL_RGBA;
		break;

	default:
		__debugbreak();
	}

	Load(format, data, width, height, format, GL_UNSIGNED_BYTE, level);

	stbi_image_free(data);
	return true;
}


void GLTexture2D::SetWrapS(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter));
}

void GLTexture2D::SetWrapT(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter));
}

void GLTexture2D::SetWrapST(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter));
}

void GLTexture2D::SetMinFilter(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter));
}

void GLTexture2D::SetMagFilter(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter));
}

void GLTexture2D::SetMinMagFilter(GLuint parameter) const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter));
}


void GLTexture2D::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void GLTexture2D::Unbind(unsigned int slot)
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}