#include "MotionBlurPass.h"

MotionBlurPass::MotionBlurPass(
	Renderer& renderer, std::shared_ptr<GLShader> shader,
	std::shared_ptr<GLTexture2D> inputTexture,
	std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture,
	std::shared_ptr<GLTexture2D> outputTexture)
	: RenderPass(renderer),
	m_Shader(shader),
	m_InputTexture(inputTexture),
	m_ClipSpaceVelocityTexture(clipSpaceVelocityTexture),
	m_FullscreenMesh(shader)
{
	GLCall(glGenFramebuffers(1, &m_Framebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));

	GLCall(glGenRenderbuffers(1, &m_Depthbuffer));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_Depthbuffer));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH32F_STENCIL8, g_WindowWidth, g_WindowHeight));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_Depthbuffer));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture->RendererID(), 0));
	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	GLCall(glDrawBuffers((sizeof(attachments) / sizeof(GLenum)), attachments));
}

MotionBlurPass::~MotionBlurPass()
{
	GLCall(glDeleteRenderbuffers(1, &m_Depthbuffer));
	m_Depthbuffer = 0;

	GLCall(glDeleteFramebuffers(1, &m_Framebuffer));
	m_Framebuffer = 0;
}


void MotionBlurPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_InputTexture->Bind(0);
	m_ClipSpaceVelocityTexture->Bind(1);

	m_Shader->SetUniform1f("u_VelocityScale", g_MotionBlurVelocityScale);

	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}