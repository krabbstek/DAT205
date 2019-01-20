#include "Dissolve2DTest.h"

#include <imgui.h>

#define DISSOLVE_TEXTURE_RESOLUTION 20

float currentTime;

float vertices[] =
{
	-0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.0f, 1.0f,
};

unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3,
};

Dissolve2DTest::Dissolve2DTest()
	: m_IBO(indices, sizeof(indices) / sizeof(unsigned int)), m_VBO(vertices, sizeof(vertices)), m_VAO()
{
	core::GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	layout.Push(GL_FLOAT, 2);
	m_VBO.SetVertexBufferLayout(layout);
	m_VAO.AddVertexBuffer(m_VBO);

	m_Texture.LoadFromFile("../res/textures/Test.png");
	m_Texture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	m_Texture.SetMagFilter(GL_LINEAR);
	m_Texture.SetWrapST(GL_CLAMP_TO_EDGE);

	unsigned char dissolveTextureData[4 * DISSOLVE_TEXTURE_RESOLUTION * DISSOLVE_TEXTURE_RESOLUTION];
	srand(time(NULL));
	for (int i = 0; i < sizeof(dissolveTextureData) / sizeof(float); i++)
	{
		dissolveTextureData[4 * i] = rand() % 256;
		dissolveTextureData[4 * i + 1] = dissolveTextureData[4 * i];
		dissolveTextureData[4 * i + 2] = dissolveTextureData[4 * i];
		dissolveTextureData[4 * i + 3] = 255;
	}

	m_DissolveTexture.Load(GL_RGBA8, dissolveTextureData, DISSOLVE_TEXTURE_RESOLUTION, DISSOLVE_TEXTURE_RESOLUTION, GL_RGBA, GL_UNSIGNED_BYTE);
	m_DissolveTexture.SetMinFilter(GL_LINEAR_MIPMAP_LINEAR);
	m_DissolveTexture.SetMagFilter(GL_LINEAR);
	m_DissolveTexture.SetWrapST(GL_CLAMP_TO_EDGE);

	m_Dissolve2DShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/Dissolve2DTest/dissolve2d_vs.glsl");
	m_Dissolve2DShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/Dissolve2DTest/dissolve2d_fs.glsl");
	m_Dissolve2DShader.CompileShaders();

	m_Dissolve2DShader.SetUniformMat4("u_MVP", core::mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));
	m_Dissolve2DShader.SetUniform4f("u_EmissionColor", core::vec4(0.2f, 0.3f, 0.8f, 1.0f));

	currentTime = -0.2f;
}

Dissolve2DTest::~Dissolve2DTest()
{
}


void Dissolve2DTest::Update(float deltaTime)
{
	currentTime += deltaTime;
	m_Dissolve2DShader.SetUniform1f("u_DissolveThreshold", currentTime);
	m_Dissolve2DShader.SetUniform1f("u_EmissionThreshold", currentTime + m_EmissionThreshold);
}

void Dissolve2DTest::Render()
{
	m_Dissolve2DShader.Bind();
	m_Dissolve2DShader.SetUniform4f("u_EmissionColor", m_EmissionColor);
	m_Texture.Bind(0);
	m_DissolveTexture.Bind(1);
	m_VAO.Bind();
	m_IBO.Bind();

	glDrawElements(GL_TRIANGLES, m_IBO.Count(), GL_UNSIGNED_INT, 0);
}

void Dissolve2DTest::ImGuiRender()
{
	if (ImGui::Button("Reset time"))
		currentTime = -0.2f;
	ImGui::SliderFloat("Emission threshold", &m_EmissionThreshold, 0.001f, 1.0f, "%.4f", 2.0f);
	ImGui::ColorPicker3("Emission color", &m_EmissionColor.r);
}