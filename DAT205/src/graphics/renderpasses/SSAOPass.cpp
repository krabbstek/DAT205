#include "SSAOPass.h"

#include "math/math_util.h"

SSAOPass::SSAOPass(
	Renderer& renderer, std::shared_ptr<GLShader> shader,
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture,
	std::shared_ptr<GLTexture2D> viewSpaceNormalTexture,
	std::shared_ptr<GLTexture2D> ssaoTexture)
	: RenderPass(renderer, shader),
	m_ViewSpacePositionTexture(viewSpacePositionTexture),
	m_ViewSpaceNormalTexture(viewSpaceNormalTexture),
	m_RandomAnglesTexture(std::make_shared<GLTexture2D>())
{
	GLCall(glGenFramebuffers(1, &m_Framebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture->RendererID(), 0));

	GLenum attachment = GL_COLOR_ATTACHMENT0;
	GLCall(glDrawBuffers(1, &attachment));

	// Init SSAO
	constexpr int numSamples = 64;
	vec3 s[numSamples];
	for (int i = 0; i < sizeof(s) / sizeof(vec3); i++)
		s[i] = CosineSampleHemisphere() * RandF();

	float randomAngles[64 * 64];
	for (int i = 0; i < 64 * 64; i++)
		randomAngles[i] = 2 * PI * RandF();

	m_RandomAnglesTexture->Load(GL_R32F, randomAngles, 64, 64, GL_RED, GL_FLOAT);
	m_RandomAnglesTexture->SetMinMagFilter(GL_NEAREST);
	m_RandomAnglesTexture->SetWrapST(GL_REPEAT);

	m_Shader->SetUniform3fv("u_Samples", s, numSamples);
	m_Shader->SetUniform1i("u_NumSamples", numSamples);

	m_Shader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));

	m_Shader->SetUniformMat4("u_ProjMatrix", m_Renderer.camera.projectionMatrix);
	m_Shader->SetUniformMat4("u_InverseProjMatrix", mat4::Inverse(m_Renderer.camera.projectionMatrix));
}

SSAOPass::~SSAOPass()
{
	GLCall(glDeleteFramebuffers(1, &m_Framebuffer));
	m_Framebuffer = 0;
}


void SSAOPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_ViewSpacePositionTexture->Bind(10);
	m_ViewSpaceNormalTexture->Bind(11);
	m_RandomAnglesTexture->Bind(12);

	m_Shader->SetUniform1f("u_KernelSize", g_SSAOKernelSize);
	m_Shader->SetUniform1f("u_Radius", g_SSAORadius);

	m_FullscreenMesh.Render(m_Renderer, *m_Shader);

	GLCall(glEnable(GL_DEPTH_TEST));
}