#include "LightingPass.h"

#include "graphics/opengl/OpenGL.h"

LightingPass::LightingPass(
	Renderer& renderer, std::shared_ptr<GLShader> shader,
	GLuint sharedDepthbuffer,
	std::shared_ptr<GLTexture2D> colorTexture,
	std::shared_ptr<GLTexture2D> irradianceMap,
	std::shared_ptr<GLTexture2D> reflectionMap,
	std::shared_ptr<GLTexture2D> ssaoTexture,
	std::shared_ptr<GLTexture2D> bloomTexture,
	std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
	std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
	std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO)
	: RenderPass(renderer, shader),
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
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bloomTexture->RendererID(), 0));

	GLenum attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	GLCall(glDrawBuffers(sizeof(attachments) / sizeof(GLenum), attachments));

	m_Shader->SetUniform1i("u_TileSize", g_TileSize);
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

	m_LightSSBO->Bind(3);
	m_LightIndexSSBO->Bind(4);
	m_TileIndexSSBO->Bind(5);

	m_IrradianceMap->Bind(7);
	m_ReflectionMap->Bind(8);
	m_SSAOTexture->Bind(9);

	m_Shader->SetUniformMat4("u_ViewInverse", mat4::Inverse(m_Renderer.camera.GetInverseViewMatrix()));
	m_Shader->SetUniform1f("u_EnvironmentMultiplier", g_EnvironmentMultiplier);
	m_Shader->SetUniform4f("u_Light.viewSpacePosition", g_GlobalLight.viewSpacePosition);
	m_Shader->SetUniform4f("u_Light.color", g_GlobalLight.color);
	m_Shader->SetUniform1f("u_BloomThreshold", g_BloomThreshold);

	RenderPass::Render(renderables);
}