#pragma once

#include "../Test.h"

#include <Core.h>

class Dissolve2DTest : public Test
{
public:
	Dissolve2DTest();
	~Dissolve2DTest();

	void Update(float deltaTime);
	void Render();
	void ImGuiRender();

	static Test* Create() { return new Dissolve2DTest(); }

private:
	float m_EmissionThreshold;
	core::vec3 m_EmissionColor;
	core::GLShader m_Dissolve2DShader;
	core::GLIndexBuffer m_IBO;
	core::GLVertexBuffer m_VBO;
	core::GLVertexArray m_VAO;
	core::GLTexture2D m_Texture;
	core::GLTexture2D m_DissolveTexture;
};