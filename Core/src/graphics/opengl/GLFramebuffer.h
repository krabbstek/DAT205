#pragma once

#include "Common.h"

#include "GLTexture2D.h"

#include <GL/glew.h>

namespace core {

	class CORE_API GLFramebuffer
	{
	public:
		GLFramebuffer(unsigned int width, unsigned int height);
		~GLFramebuffer();

		GLTexture2D* AttachRGBColorTexture() const;

		void GenerateDepthStencilRenderbuffer();

		void ClearColor(float r, float g, float b, float a) const;
		static void Clear(GLuint mask);

		void Bind() const;
		static void Unbind();

	private:
		unsigned int m_Width, m_Height;
		GLuint m_FramebufferRendererID;
		GLuint m_DepthStencilRenderbufferID;
	};

} 