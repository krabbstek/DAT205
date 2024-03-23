#include "BackgroundPrepass.h"

BackgroundPrepass::BackgroundPrepass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture)
	: RenderPass(renderer), m_FullscreenMesh(shader), m_Shader(shader)
{
	GLCall(glGenFramebuffers(1, &m_Framebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, clipSpaceVelocityTexture->RendererID(), 0));
	GLenum attachment = GL_COLOR_ATTACHMENT0;
	GLCall(glDrawBuffers(1, &attachment));
}

BackgroundPrepass::~BackgroundPrepass()
{
	GLCall(glDeleteFramebuffers(1, &m_Framebuffer));
	m_Framebuffer = 0;
}

void BackgroundPrepass::Render(std::vector<Renderable*>&)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_Shader->SetUniformMat4("u_InvVP", mat4::Inverse(m_Renderer.camera.projectionMatrix * m_Renderer.camera.GetViewMatrix()));
	m_Shader->SetUniformMat4("u_PrevVP", m_Renderer.camera.projectionMatrix * m_Renderer.camera.GetPreviousViewMatrix());

	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}