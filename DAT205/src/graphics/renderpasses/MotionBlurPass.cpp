#include "MotionBlurPass.h"

MotionBlurPass::MotionBlurPass(
	Renderer& renderer,
	std::shared_ptr<GLShader> maxTileVelocityShader,
	std::shared_ptr<GLShader> velocityVarianceShader,
	std::shared_ptr<GLShader> motionBlurShader,
	std::shared_ptr<GLTexture2D> inputTexture,
	std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture,
	std::shared_ptr<GLTexture2D> outputTexture)
	: RenderPass(renderer),
	m_MaxTileVelocityShader(maxTileVelocityShader),
	m_VelocityVarianceShader(velocityVarianceShader),
	m_MotionBlurShader(motionBlurShader),
	m_InputTexture(inputTexture),
	m_ClipSpaceVelocityTexture(clipSpaceVelocityTexture),
	m_FullscreenMesh(maxTileVelocityShader)
{
	GLenum attachment = GL_COLOR_ATTACHMENT0;

	// First max tile velocity
	GLCall(glGenFramebuffers(1, &m_MaxTileVelocityFramebuffer[0]));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MaxTileVelocityFramebuffer[0]));

	m_MaxTileVelocityTexture[0] = std::make_shared<GLTexture2D>();
	m_MaxTileVelocityTexture[0]->Load(GL_RG16F, nullptr, g_WindowWidth, g_WindowHeight / g_VelocityTileSize, GL_RG, GL_FLOAT);
	m_MaxTileVelocityTexture[0]->SetMinMagFilter(GL_NEAREST);
	m_MaxTileVelocityTexture[0]->SetWrapST(GL_CLAMP_TO_BORDER);

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MaxTileVelocityTexture[0]->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	// Second max tile velocity
	GLCall(glGenFramebuffers(1, &m_MaxTileVelocityFramebuffer[1]));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MaxTileVelocityFramebuffer[1]));

	m_MaxTileVelocityTexture[1] = std::make_shared<GLTexture2D>();
	m_MaxTileVelocityTexture[1]->Load(GL_RG16F, nullptr, g_WindowWidth / g_VelocityTileSize, g_WindowHeight / g_VelocityTileSize, GL_RG, GL_FLOAT);
	m_MaxTileVelocityTexture[1]->SetMinMagFilter(GL_NEAREST);
	m_MaxTileVelocityTexture[1]->SetWrapST(GL_CLAMP_TO_BORDER);

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_MaxTileVelocityTexture[1]->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	// Velocity variance
	GLCall(glGenFramebuffers(1, &m_VelocityVarianceFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_VelocityVarianceFramebuffer));

	m_VelocityVarianceTexture = std::make_shared<GLTexture2D>();
	m_VelocityVarianceTexture->Load(GL_RG16F, nullptr, g_WindowWidth / g_VelocityTileSize, g_WindowHeight / g_VelocityTileSize, GL_RG, GL_FLOAT);
	m_VelocityVarianceTexture->SetMinMagFilter(GL_NEAREST);
	m_VelocityVarianceTexture->SetWrapST(GL_CLAMP_TO_BORDER);

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_VelocityVarianceTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));

	// Output framebuffer
	GLCall(glGenFramebuffers(1, &m_OutputFramebuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_OutputFramebuffer));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture->RendererID(), 0));
	GLCall(glDrawBuffers(1, &attachment));
}

MotionBlurPass::~MotionBlurPass()
{
	GLCall(glDeleteFramebuffers(2, m_MaxTileVelocityFramebuffer));
	m_MaxTileVelocityFramebuffer[0] = 0;
	m_MaxTileVelocityFramebuffer[1] = 0;

	GLCall(glDeleteFramebuffers(1, &m_VelocityVarianceFramebuffer));
	m_VelocityVarianceFramebuffer = 0;
	
	GLCall(glDeleteFramebuffers(1, &m_OutputFramebuffer));
	m_OutputFramebuffer = 0;
}


void MotionBlurPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MaxTileVelocityFramebuffer[0]));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight / g_VelocityTileSize));
	GLCall(glDisable(GL_DEPTH_TEST));

	m_ClipSpaceVelocityTexture->Bind(0);
	m_MaxTileVelocityShader->SetUniform1i("u_VerticalSampling", 1);
	m_FullscreenMesh.SetMainShader(m_MaxTileVelocityShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_MaxTileVelocityFramebuffer[1]));
	GLCall(glViewport(0, 0, g_WindowWidth / g_VelocityTileSize, g_WindowHeight / g_VelocityTileSize));

	m_MaxTileVelocityTexture[0]->Bind(0);
	m_MaxTileVelocityShader->SetUniform1i("u_VerticalSampling", 0);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_VelocityVarianceFramebuffer));

	m_MaxTileVelocityTexture[1]->Bind(0);
	m_FullscreenMesh.SetMainShader(m_VelocityVarianceShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_OutputFramebuffer));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));

	m_InputTexture->Bind(0);
	m_ClipSpaceVelocityTexture->Bind(1);
	m_MaxTileVelocityTexture[1]->Bind(2);
	m_MotionBlurShader->SetUniform1f("u_VelocityScale", g_MotionBlurVelocityScale);
	m_FullscreenMesh.SetMainShader(m_MotionBlurShader);
	m_FullscreenMesh.Render(m_Renderer);

	GLCall(glEnable(GL_DEPTH_TEST));
}