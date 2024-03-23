#include "LightingPass.h"

#include "graphics/opengl/OpenGL.h"

LightingPass::LightingPass(
	Renderer& renderer,
	std::shared_ptr<GLShader> lightingPassShader,
	GLuint sharedDepthbuffer,
	std::shared_ptr<GLTexture2D> colorTexture,
	std::shared_ptr<GLTexture2D> irradianceMap,
	std::shared_ptr<GLTexture2D> reflectionMap,
	std::shared_ptr<GLTexture2D> ssaoTexture,
	std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
	std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
	std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO)
	: RenderPass(renderer),
	m_LightingPassShader(lightingPassShader),
	m_LightSSBO(lightSSBO),
	m_LightIndexSSBO(lightIndexSSBO),
	m_TileIndexSSBO(tileIndexSSBO),
	m_IrradianceMap(irradianceMap),
	m_ReflectionMap(reflectionMap),
	m_SSAOTexture(ssaoTexture)
{
	GLCall(glGenFramebuffers(1, &m_Framebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, sharedDepthbuffer));
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, sharedDepthbuffer));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture->RendererID(), 0));

	GLenum attachment = GL_COLOR_ATTACHMENT0;
	GLCall(glDrawBuffers(1, &attachment));

	m_LightingPassShader->SetUniform1i("u_TileSize", g_TileSize);
}

LightingPass::~LightingPass()
{
	GLCall(glDeleteFramebuffers(1, &m_Framebuffer));
	m_Framebuffer = 0;
}


void LightingPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));

	m_LightingPassShader->SetUniform1i("u_NumTileCols", g_NumTileCols);
	m_LightingPassShader->SetUniform1i("u_TileSize", g_TileSize);
	m_LightingPassShader->SetUniform1i("u_MaxNumLightsPerTile", g_MaxNumLightsPerTile);
	m_LightingPassShader->SetUniform1f("u_EnvironmentMultiplier", g_EnvironmentMultiplier);
	m_LightingPassShader->SetUniformMat4("u_ViewInverse", m_Renderer.camera.GetInverseViewMatrix());

	m_LightSSBO->Bind(3);
	m_LightIndexSSBO->Bind(4);
	m_TileIndexSSBO->Bind(5);

	m_IrradianceMap->Bind(7);
	m_ReflectionMap->Bind(8);
	m_SSAOTexture->Bind(9);

	RenderPass::Render(renderables);
}