#include "pch.h"

#include "GLFramebuffer.h"

#include "GLCommon.h"

namespace core {

	GLFramebuffer::GLFramebuffer(unsigned int width, unsigned int height)
		: m_Width(width), m_Height(height)
	{
		GLCall(glGenFramebuffers(1, &m_FramebufferRendererID));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CORE_ERROR("Failed to generate framebuffer!");
			__debugbreak();
		}
	}

	GLFramebuffer::~GLFramebuffer()
	{
		if (m_FramebufferRendererID)
		{
			GLCall(glDeleteFramebuffers(1, &m_FramebufferRendererID));
		}
		if (m_DepthStencilRenderbufferID)
		{
			GLCall(glDeleteRenderbuffers(1, &m_DepthStencilRenderbufferID));
		}
	}


	GLTexture2D* GLFramebuffer::AttachTexture(GLuint type, unsigned int attachment /*= 0*/) const
	{
		GLint fbDim[4];
		GLCall(glGetIntegerv(GL_VIEWPORT, fbDim));

		GLTexture2D* texture = new GLTexture2D();
		texture->Load(type, NULL, /*width =*/ fbDim[2], /*height =*/ fbDim[3], type, GL_UNSIGNED_BYTE);
		texture->SetMinMagFilter(GL_LINEAR);

		Bind();
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachment, GL_TEXTURE_2D, texture->m_RendererID, 0));

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CORE_ERROR("Failed to attach color buffer!");
			__debugbreak();
		}

		return texture;
	}


	void GLFramebuffer::GenerateDepthStencilRenderbuffer()
	{
		if (m_DepthStencilRenderbufferID)
		{
			GLCall(glDeleteRenderbuffers(1, &m_DepthStencilRenderbufferID));
		}

		GLint fbDim[4];
		GLCall(glGetIntegerv(GL_VIEWPORT, fbDim));

		GLCall(glGenRenderbuffers(1, &m_DepthStencilRenderbufferID));
		GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRenderbufferID));
		GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, /*width =*/ fbDim[2], /*height =*/ fbDim[3]));

		Bind();
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRenderbufferID);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			CORE_ERROR("Failed to generate depth stencil buffer!");
			__debugbreak();
		}
	}


	void GLFramebuffer::ClearColor(float r, float g, float b, float a) const
	{
		Bind();
		GLCall(glClearColor(r, g, b, a));
	}

	void GLFramebuffer::Clear(GLuint mask)
	{
		GLCall(glClear(mask));
	}


	void GLFramebuffer::Bind() const
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferRendererID));
	}

	void GLFramebuffer::Unbind()
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}

}