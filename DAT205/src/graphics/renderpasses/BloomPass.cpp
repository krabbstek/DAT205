#include "BloomPass.h"

BloomPass::BloomPass(Renderer& renderer,
	std::shared_ptr<GLShader> bloomBrightnessShader,
	std::shared_ptr<GLShader> bloomShader,
	std::shared_ptr<GLShader> blur1DShader,
	std::shared_ptr<GLTexture2D> inputColorTexture,
	std::shared_ptr<GLTexture2D> bloomOutputTexture)
	: RenderPass(renderer),
	m_BloomBrightnessShader(bloomBrightnessShader),
	m_BloomShader(bloomShader),
	m_Blur1DShader(blur1DShader),
	m_InputColorTexture(inputColorTexture),
	m_BloomInputTexture(std::make_shared<GLTexture2D>()),
	m_BloomIntermediateTexture(std::make_shared<GLTexture2D>()),
	m_BloomOutputTexture(bloomOutputTexture),
	m_FullscreenMesh(blur1DShader)
{
	m_BloomInputTexture->Load(GL_RGB16F, nullptr, g_BloomTextureWidth, g_BloomTextureHeight, GL_RGB, GL_FLOAT);
	m_BloomInputTexture->SetWrapST(GL_CLAMP_TO_BORDER);
	m_BloomInputTexture->SetMinMagFilter(GL_NEAREST);

	m_BloomIntermediateTexture->Load(GL_RGB16F, nullptr, g_BloomTextureWidth, g_BloomTextureHeight, GL_RGB, GL_FLOAT);
	m_BloomIntermediateTexture->SetWrapST(GL_CLAMP_TO_BORDER);
	m_BloomIntermediateTexture->SetMinMagFilter(GL_NEAREST);

	GLenum attachment = GL_COLOR_ATTACHMENT0;

	GLCall(glGenFramebuffers(1, &m_BloomInputFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomInputFramebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomInputTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	GLCall(glGenFramebuffers(3, m_BlurFramebuffer));

	for (int i = 0; i < 3; i++)
	{
		m_BloomBlurredTexture[i] = std::make_shared<GLTexture2D>();
		m_BloomBlurredTexture[i]->Load(GL_RGB16F, nullptr, g_BloomTextureWidth, g_BloomTextureHeight, GL_RGB, GL_FLOAT);
		m_BloomBlurredTexture[i]->SetWrapST(GL_CLAMP_TO_BORDER);
		m_BloomBlurredTexture[i]->SetMinMagFilter(GL_LINEAR);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer[i]));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomBlurredTexture[i]->RendererID(), 0));
		GLCall(glDrawBuffers(1, &attachment));

		GLenum status;
		GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::printf("Framebuffer incomplete!\n");
			__debugbreak();
		}
	}

	GLCall(glGenFramebuffers(1, &m_BlurIntermediateFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurIntermediateFramebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomIntermediateTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	GLenum status;
	GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::printf("Framebuffer incomplete!\n");
		__debugbreak();
	}

	GLCall(glGenFramebuffers(1, &m_OutputFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_OutputFramebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomOutputTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::printf("Framebuffer incomplete!\n");
		__debugbreak();
	}
}

BloomPass::~BloomPass()
{
	GLCall(glDeleteFramebuffers(3, m_BlurFramebuffer));
	m_BlurFramebuffer[0] = 0;
	m_BlurFramebuffer[1] = 0;
	m_BlurFramebuffer[2] = 0;

	GLCall(glDeleteFramebuffers(1, &m_BlurIntermediateFramebuffer));
	m_BlurIntermediateFramebuffer = 0;

	GLCall(glDeleteFramebuffers(1, &m_OutputFramebuffer));
	m_OutputFramebuffer = 0;
}


void BloomPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BloomInputFramebuffer));
	GLCall(glViewport(0, 0, g_BloomTextureWidth, g_BloomTextureHeight));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_BloomBrightnessShader->Bind();
	m_BloomBrightnessShader->SetUniform1f("u_BloomThreshold", g_BloomThreshold);
	m_InputColorTexture->GenerateMipmap();
	m_InputColorTexture->Bind(0);
	m_FullscreenMesh.SetMainShader(m_BloomBrightnessShader);
	m_FullscreenMesh.Render(m_Renderer);

	m_Blur1DShader->SetUniform2f("u_ViewportSize", float(g_BloomTextureWidth), float(g_BloomTextureHeight));
	m_FullscreenMesh.SetMainShader(m_Blur1DShader);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurIntermediateFramebuffer));
	m_Blur1DShader->SetUniform1i("u_VerticalBlur", 0);
	m_BloomInputTexture->Bind(0);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer[0]));
	m_Blur1DShader->SetUniform1i("u_VerticalBlur", 1);
	m_BloomIntermediateTexture->Bind(0);
	m_FullscreenMesh.Render(m_Renderer);

	for (int i = 1; i < 3; i++)
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurIntermediateFramebuffer));
		m_Blur1DShader->SetUniform1i("u_VerticalBlur", 0);
		m_BloomBlurredTexture[i - 1]->Bind(0);
		m_FullscreenMesh.Render(m_Renderer);

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer[i]));
		m_Blur1DShader->SetUniform1i("u_VerticalBlur", 1);
		m_BloomIntermediateTexture->Bind(0);
		m_FullscreenMesh.Render(m_Renderer);
	}

	m_FullscreenMesh.SetMainShader(m_BloomShader);
	m_BloomShader->SetUniform1f("u_BloomAlpha", g_BloomAlpha);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_OutputFramebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));
	m_InputColorTexture->Bind(0);
	for (int i = 0; i < 3; i++)
		if (g_UseBloomLevelTexture[i])
			m_BloomBlurredTexture[i]->Bind(i + 1);
		else
			GLTexture2D::Unbind(i + 1);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}