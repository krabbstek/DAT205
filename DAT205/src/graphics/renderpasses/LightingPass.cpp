#include "LightingPass.h"

#include "graphics/opengl/OpenGL.h"

LightingPass::LightingPass(
	Renderer& renderer, std::shared_ptr<GLShader> shader,
	std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
	std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
	std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO,
	std::shared_ptr<GLTexture2D> irradianceMap,
	std::shared_ptr<GLTexture2D> reflectionMap)
	: RenderPass(renderer, shader),
	m_LightSSBO(lightSSBO),
	m_LightIndexSSBO(lightIndexSSBO),
	m_TileIndexSSBO(tileIndexSSBO),
	m_IrradianceMap(irradianceMap),
	m_ReflectionMap(reflectionMap)
{
	m_Shader->SetUniform1i("u_TileSize", g_TileSize);
}


void LightingPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));

	m_LightSSBO->Bind(3);
	m_LightIndexSSBO->Bind(4);
	m_TileIndexSSBO->Bind(5);

	m_IrradianceMap->Bind(7);
	m_ReflectionMap->Bind(8);

	m_Shader->SetUniformMat4("u_ViewInverse", mat4::Inverse(m_Renderer.camera.GetViewMatrix()));
	m_Shader->SetUniform1f("u_EnvironmentMultiplier", g_EnvironmentMultiplier);
	m_Shader->SetUniform4f("u_Light.viewSpacePosition", g_GlobalLight.viewSpacePosition);
	m_Shader->SetUniform4f("u_Light.color", g_GlobalLight.color);

	RenderPass::Render(renderables);
}