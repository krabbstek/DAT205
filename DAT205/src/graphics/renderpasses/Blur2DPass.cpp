#include "Blur2DPass.h"

Blur2DPass::Blur2DPass(Renderer& renderer, std::shared_ptr<GLShader> shader,
	std::shared_ptr<GLTexture2D> inputTexture,
	std::shared_ptr<GLTexture2D> outputTexture)
	: RenderPass(renderer, shader),
	m_InputTexture(inputTexture),
	m_OutputTexture(outputTexture),
	m_IntermediateTexture(std::make_shared<GLTexture2D>()),
	m_FullscreenMesh(shader)
{
	m_IntermediateTexture->Load(GL_RGBA32F, nullptr, g_WindowWidth, g_WindowHeight, GL_RGBA, GL_UNSIGNED_BYTE);
	m_IntermediateTexture->SetMinMagFilter(GL_NEAREST);
	m_IntermediateTexture->SetWrapST(GL_CLAMP_TO_EDGE);

	GLCall(glGenFramebuffers(1, &m_Framebuffer1));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer1));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IntermediateTexture->RendererID(), 0));
	GLenum attachment = GL_COLOR_ATTACHMENT0;
	GLCall(glDrawBuffers(1, &attachment));

	GLCall(glGenFramebuffers(1, &m_Framebuffer2));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer2));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_OutputTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));
}

Blur2DPass::~Blur2DPass()
{
	GLCall(glDeleteFramebuffers(1, &m_Framebuffer1));
	m_Framebuffer1 = 0;

	GLCall(glDeleteFramebuffers(1, &m_Framebuffer2));
	m_Framebuffer2 = 0;
}


void Blur2DPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glViewport(0, 0, m_InputTexture->GetWidth(), m_InputTexture->GetHeight()));
	GLCall(glDisable(GL_DEPTH_TEST));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer1));
	m_InputTexture->Bind(0);
	m_Shader->SetUniform1i("u_VerticalBlur", 0);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer2));
	m_IntermediateTexture->Bind(0);
	m_Shader->SetUniform1i("u_VerticalBlur", 1);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}