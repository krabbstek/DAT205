#pragma once

#include "../Test.h"

class GaussianBlurTest : public Test
{
public:
	GaussianBlurTest(unsigned int width, unsigned int height);
	~GaussianBlurTest();

	void Update(float deltaTime);
	void Render();
	void ImGuiRender();

	inline static Test* Create() { return new GaussianBlurTest(core::Application::GetWindow()->GetWidth(), core::Application::GetWindow()->GetHeight()); }

private:
	unsigned int m_Width, m_Height;
	core::GLFramebuffer m_NonBlurredFramebuffer;
	core::GLFramebuffer m_IntermediateBlurredFramebuffer;
	core::GLVertexBuffer m_BoxVBO;
	core::GLVertexBuffer m_FullscreenVBO;
	core::GLVertexArray m_BoxVAO;
	core::GLVertexArray m_FullscreenVAO;
	core::GLIndexBuffer m_BoxIBO;
	core::GLIndexBuffer m_FullscreenIBO;
	core::GLShader m_Shader;
	core::GLShader m_BlurShader;
	core::GLTexture2D m_BoxTexture;
	core::GLTexture2D m_NonBlurredTexture;
	core::GLTexture2D m_IntermediateBlurredTexture;
};