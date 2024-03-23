#pragma once

#include "../Test.h"

#include <Core.h>

class DissolveBloom2DTest : public Test
{
public:
	DissolveBloom2DTest(unsigned int width, unsigned int height);
	~DissolveBloom2DTest();

	void Update(float deltaTime);
	void Render();
	void ImGuiRender();

	inline static Test* Create() { return new DissolveBloom2DTest(core::Application::GetWindow()->GetWidth(), core::Application::GetWindow()->GetHeight()); }

private:
	bool m_Paused;
	unsigned int m_Width;
	unsigned int m_Height;
	float m_EmissionThreshold;
	float m_EmissionIntensity;
	core::vec4 m_EmissionColor;
	core::GLFramebuffer m_RawFramebuffer;
	core::GLFramebuffer m_IntermediateBloomFramebuffer;
	core::GLFramebuffer m_CompleteBloomFramebuffer;
	core::GLFramebuffer m_MergedFramebuffer;
	core::GLTexture2D m_BoxTexture;
	core::GLTexture2D m_RawFramebufferColorTexture;
	core::GLTexture2D m_RawFramebufferBloomTexture;
	core::GLTexture2D m_IntermediateBloomFramebufferTexture;
	core::GLTexture2D m_CompleteBloomFramebufferTexture;
	core::GLTexture2D m_MergedFramebufferTexture;
	core::GLTexture2D m_DissolveTexture;
	core::GLVertexBuffer m_BoxVBO;
	core::GLVertexBuffer m_FullscreenVBO;
	core::GLVertexArray m_BoxVAO;
	core::GLVertexArray m_FullscreenVAO;
	core::GLIndexBuffer m_BoxIBO;
	core::GLIndexBuffer m_FullscreenIBO;
	core::GLShader m_DissolveShader;
	core::GLShader m_BloomShader;
	core::GLShader m_MergeShader;
	core::GLShader m_FullscreenShader;
};