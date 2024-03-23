#include "BackgroundPass.h"

BackgroundPass::BackgroundPass(Renderer& renderer, std::shared_ptr<GLShader> shader, GLuint targetFramebuffer, std::shared_ptr<GLTexture2D> environmentMap)
	: RenderPass(renderer),
	m_Shader(shader),
	m_TargetFramebuffer(targetFramebuffer),
	m_EnvironmentMap(environmentMap),
	m_FullscreenMesh(shader)
{
}


void BackgroundPass::Render(std::vector<Renderable*>&)
{
	m_Shader->Bind();
	m_Shader->SetUniform1f("u_EnvironmentMultiplier", g_EnvironmentMultiplier);
	m_Shader->SetUniformMat4("u_InvVP", mat4::Inverse(m_Renderer.camera.projectionMatrix * m_Renderer.camera.GetViewMatrix()));
	m_Shader->SetUniform3f("u_CameraPos", m_Renderer.camera.position);

	m_EnvironmentMap->Bind(6);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_TargetFramebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));

	GLCall(glDisable(GL_DEPTH_TEST));
	m_FullscreenMesh.Render(m_Renderer);
	GLCall(glEnable(GL_DEPTH_TEST));
}