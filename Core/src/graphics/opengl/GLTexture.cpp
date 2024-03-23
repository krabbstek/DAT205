#include "pch.h"

#include "GLTexture.h"

#include "GLCommon.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace core {

	GLTexture::GLTexture(const char* filePath, GLint format)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(filePath, &width, &height, &nrChannels, 0);
		if (!data)
		{
			CORE_ERROR("Failed to load image in GLTexture constructor!");
			m_RendererID = -1;
			return;
		}

		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, format, GL_UNSIGNED_BYTE, data));
		STBI_FREE(data);
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}

	GLTexture::~GLTexture()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}


	void GLTexture::Bind(unsigned int slot) const
	{
		GLCall(glActiveTexture(GL_TEXTURE0 + slot));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void GLTexture::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}


	void GLTexture::Init()
	{
		stbi_set_flip_vertically_on_load(true);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	}

}