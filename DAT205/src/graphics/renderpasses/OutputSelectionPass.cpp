#include "OutputSelectionPass.h"

OutputSelectionPass::OutputSelectionPass(Renderer& renderer, std::shared_ptr<GLShader> shader,
	const OUTPUT_SELECTION& outputSelection,
	std::shared_ptr<GLShader> depthShader,
	std::shared_ptr<GLShader> fullscreenShader,
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture,
	std::shared_ptr<GLTexture2D> viewSpaceNormalTexture,
	std::shared_ptr<GLTexture2D> ssaoTexture,
	std::shared_ptr<GLTexture2D> lightingColorTexture,
	std::shared_ptr<GLTexture2D> motionBlurredTexture,
	std::shared_ptr<GLTexture2D> bloomTexture)
	: RenderPass(renderer, shader),
	m_OutputSelection(outputSelection),
	m_DepthShader(depthShader),
	m_FullscreenShader(fullscreenShader),
	m_ViewSpacePositionTexture(viewSpacePositionTexture),
	m_ViewSpaceNormalTexture(viewSpaceNormalTexture),
	m_SSAOTexture(ssaoTexture),
	m_LightingColorTexture(lightingColorTexture),
	m_MotionBlurredTexture(motionBlurredTexture),
	m_BloomTexture(bloomTexture),
	m_FullscreenMesh(depthShader)
{
}


void OutputSelectionPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));

	GLCall(glDisable(GL_DEPTH_TEST));

	switch (m_OutputSelection)
	{
	case OUTPUT_SELECTION_DEPTH:
		m_ViewSpacePositionTexture->Bind(0);
		m_FullscreenMesh.SetMainShader(m_DepthShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;

	case OUTPUT_SELECTION_NORMAL:
		m_ViewSpaceNormalTexture->Bind(0);
		m_FullscreenShader->SetUniform1i("u_TextureData", OUTPUT_RGB);
		m_FullscreenMesh.SetMainShader(m_FullscreenShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;

	case OUTPUT_SELECTION_SSAO:
		m_SSAOTexture->Bind(0);
		m_FullscreenShader->SetUniform1i("u_TextureData", OUTPUT_R);
		m_FullscreenMesh.SetMainShader(m_FullscreenShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;

	case OUTPUT_SELECTION_LIGHTING:
		m_LightingColorTexture->Bind(0);
		m_FullscreenShader->SetUniform1i("u_TextureData", OUTPUT_RGB);
		m_FullscreenMesh.SetMainShader(m_FullscreenShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;

	case OUTPUT_SELECTION_MOTION_BLUR:
		m_MotionBlurredTexture->Bind(0);
		m_FullscreenShader->SetUniform1i("u_TextureData", OUTPUT_RGB);
		m_FullscreenMesh.SetMainShader(m_FullscreenShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;

	case OUTPUT_SELECTION_BLOOM:
		m_BloomTexture->Bind(0);
		m_FullscreenShader->SetUniform1i("u_TextureData", OUTPUT_RGB);
		m_FullscreenMesh.SetMainShader(m_FullscreenShader);
		m_FullscreenMesh.Render(m_Renderer);
		break;
	}

	GLCall(glEnable(GL_DEPTH_TEST));
}