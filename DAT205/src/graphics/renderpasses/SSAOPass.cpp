#include "SSAOPass.h"

#include "math/math_util.h"

SSAOPass::SSAOPass(
	Renderer& renderer,
	std::shared_ptr<GLShader> shader,
	std::shared_ptr<GLShader> bilateralBlurShader,
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture,
	std::shared_ptr<GLTexture2D> viewSpaceNormalTexture,
	std::shared_ptr<GLTexture2D> ssaoTexture)
	: RenderPass(renderer),
	m_SSAOShader(shader),
	m_BilateralBlurShader(bilateralBlurShader),
	m_ViewSpacePositionTexture(viewSpacePositionTexture),
	m_ViewSpaceNormalTexture(viewSpaceNormalTexture),
	m_RandomAnglesTexture(std::make_shared<GLTexture2D>()),
	m_IntermediateTexture1(std::make_shared<GLTexture2D>()),
	m_IntermediateTexture2(std::make_shared<GLTexture2D>()),
	m_FullscreenMesh(shader)
{
	m_IntermediateTexture1->Load(GL_R16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RED, GL_FLOAT);
	m_IntermediateTexture1->SetMinMagFilter(GL_NEAREST);
	m_IntermediateTexture1->SetWrapST(GL_CLAMP_TO_EDGE);

	m_IntermediateTexture2->Load(GL_R16F, nullptr, g_WindowWidth, g_WindowHeight, GL_RED, GL_FLOAT);
	m_IntermediateTexture2->SetMinMagFilter(GL_NEAREST);
	m_IntermediateTexture2->SetWrapST(GL_CLAMP_TO_EDGE);

	GLenum attachment = GL_COLOR_ATTACHMENT0;

	GLCall(glGenFramebuffers(1, &m_SSAOFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFramebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IntermediateTexture1->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	GLCall(glGenFramebuffers(1, &m_BlurFramebuffer1));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer1));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_IntermediateTexture2->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	GLCall(glGenFramebuffers(1, &m_BlurFramebuffer2));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer2));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	// Init SSAO
	constexpr int numSamples = 16;
	vec3 s[numSamples];
	for (int i = 0; i < sizeof(s) / sizeof(vec3); i++)
		s[i] = CosineSampleHemisphere() * RandF();

	float randomAngles[64 * 64];
	for (int i = 0; i < sizeof(randomAngles) / sizeof(float); i++)
		randomAngles[i] = 2 * PI * RandF();

	m_RandomAnglesTexture->Load(GL_R16F, randomAngles, 64, 64, GL_RED, GL_FLOAT);
	m_RandomAnglesTexture->SetMinMagFilter(GL_NEAREST);
	m_RandomAnglesTexture->SetWrapST(GL_REPEAT);

	m_SSAOShader->SetUniform3fv("u_Samples", s, numSamples);
	m_SSAOShader->SetUniform1i("u_NumSamples", numSamples);
	m_SSAOShader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));
	m_SSAOShader->SetUniformMat4("u_ProjMatrix", m_Renderer.camera.projectionMatrix);
	m_SSAOShader->SetUniformMat4("u_InverseProjMatrix", mat4::Inverse(m_Renderer.camera.projectionMatrix));
	m_SSAOShader->SetUniform1i("u_RandomAnglesTextureSize", 64);

	m_BilateralBlurShader->SetUniform2f("u_InvViewportSize", 1.0f / float(g_WindowWidth), 1.0f / float(g_WindowHeight));
}

SSAOPass::~SSAOPass()
{
	GLCall(glDeleteFramebuffers(1, &m_SSAOFramebuffer));
	m_SSAOFramebuffer = 0;

	GLCall(glDeleteFramebuffers(1, &m_BlurFramebuffer1));
	m_BlurFramebuffer1 = 0;

	GLCall(glDeleteFramebuffers(1, &m_BlurFramebuffer2));
	m_BlurFramebuffer2 = 0;
}


void SSAOPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glDisable(GL_DEPTH_TEST));

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_SSAOFramebuffer));
	m_ViewSpacePositionTexture->Bind(10);
	m_ViewSpaceNormalTexture->Bind(11);
	m_RandomAnglesTexture->Bind(12);
	m_SSAOShader->SetUniform1f("u_Bias", g_SSAOBias);
	m_SSAOShader->SetUniform1f("u_Radius", g_SSAORadius);
	m_FullscreenMesh.SetMainShader(m_SSAOShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer1));
	m_BilateralBlurShader->SetUniform1i("u_SampleSize", g_SSAOBlurSampleSize);
	m_BilateralBlurShader->SetUniform1f("u_BlurSigma", g_SSAOBilateralBlurSigma);
	m_BilateralBlurShader->SetUniform1f("u_DepthSigma", g_SSAODepthSigma);
	m_BilateralBlurShader->SetUniform1i("u_VerticalBlur", 0);
	m_ViewSpacePositionTexture->Bind(10);
	m_IntermediateTexture1->Bind(11);
	m_FullscreenMesh.SetMainShader(m_BilateralBlurShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_BlurFramebuffer2));
	m_BilateralBlurShader->SetUniform1i("u_VerticalBlur", 1);
	m_ViewSpacePositionTexture->Bind(10);
	m_IntermediateTexture2->Bind(11);
	m_FullscreenMesh.SetMainShader(m_BilateralBlurShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}