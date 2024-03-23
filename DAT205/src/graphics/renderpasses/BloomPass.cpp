#include "BloomPass.h"

BloomPass::BloomPass(Renderer& renderer, std::shared_ptr<GLShader> bloomShader,
	std::shared_ptr<GLShader> blur1DShader,
	std::shared_ptr<GLTexture2D> lightingPassColorTexture,
	std::shared_ptr<GLTexture2D> bloomInputTexture,
	std::shared_ptr<GLTexture2D> bloomOutputTexture)
	: RenderPass(renderer, bloomShader),
	m_Blur1DShader(blur1DShader),
	m_LightingPassColorTexture(lightingPassColorTexture),
	m_BloomInputTexture(bloomInputTexture),
	m_BloomIntermediateTexture1(std::make_shared<GLTexture2D>()),
	m_BloomIntermediateTexture2(std::make_shared<GLTexture2D>()),
	m_BloomOutputTexture(bloomOutputTexture)
{
	m_BloomIntermediateTexture1->Load(GL_RGB32F, nullptr, g_BloomTextureWidth, g_BloomTextureHeight, GL_RGB, GL_UNSIGNED_BYTE);
	m_BloomIntermediateTexture1->SetMinMagFilter(GL_LINEAR);
	m_BloomIntermediateTexture1->SetWrapST(GL_CLAMP_TO_EDGE);

	m_BloomIntermediateTexture2->Load(GL_RGB32F, nullptr, g_BloomTextureWidth, g_BloomTextureHeight, GL_RGB, GL_UNSIGNED_BYTE);
	m_BloomIntermediateTexture2->SetMinMagFilter(GL_LINEAR);
	m_BloomIntermediateTexture2->SetWrapST(GL_CLAMP_TO_EDGE);

	GLenum attachment = GL_COLOR_ATTACHMENT0;

	GLCall(glGenFramebuffers(1, &m_Framebuffer1));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer1));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomIntermediateTexture1->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));
	GLenum status;
	GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::printf("Framebuffer incomplete!\n");
		__debugbreak();
	}

	GLCall(glGenFramebuffers(1, &m_Framebuffer2));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer2));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_BloomIntermediateTexture2->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));
	GLCall(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		std::printf("Framebuffer incomplete!\n");
		__debugbreak();
	}

	GLCall(glGenFramebuffers(1, &m_Framebuffer3));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer3));
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
	GLCall(glDeleteFramebuffers(1, &m_Framebuffer1));
	m_Framebuffer1 = 0;

	GLCall(glDeleteFramebuffers(1, &m_Framebuffer2));
	m_Framebuffer2 = 0;

	GLCall(glDeleteFramebuffers(1, &m_Framebuffer3));
	m_Framebuffer3 = 0;
}


void BloomPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glViewport(0, 0, g_BloomTextureWidth, g_BloomTextureHeight));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_BloomInputTexture->Bind(0);
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	m_Blur1DShader->SetUniform2f("u_ViewportSize", float(g_BloomTextureWidth), float(g_BloomTextureHeight));
	m_Shader->SetUniform1f("u_BloomAlpha", g_BloomAlpha);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer1));
	m_Blur1DShader->SetUniform1i("u_VerticalBlur", 0);
	m_FullscreenMesh.Render(m_Renderer, *m_Blur1DShader);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer2));
	m_BloomIntermediateTexture1->Bind(0);
	m_Blur1DShader->SetUniform1i("u_VerticalBlur", 1);
	m_FullscreenMesh.Render(m_Renderer, *m_Blur1DShader);

	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer3));
	m_LightingPassColorTexture->Bind(0);
	m_BloomIntermediateTexture2->Bind(1);
	m_FullscreenMesh.Render(m_Renderer, *m_Shader);

	GLCall(glEnable(GL_DEPTH_TEST));
}