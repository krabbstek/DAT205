#include "DissolveBloom2DTest.h"

#include <imgui.h>

#define DISSOLVE_TEXTURE_RESOLUTION 20

static float boxVertices[] =
{
	-0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.0f, 1.0f,
};

static unsigned int boxIndices[] =
{
	0, 1, 2,
	0, 2, 3,
};

static float fullscreenVertices[] =
{
	-1.0f, -1.0f,  0.0f, 0.0f,
	 3.0f, -1.0f,  2.0f, 0.0f,
	-1.0f,  3.0f,  0.0f, 2.0f,
};

static unsigned int fullscreenIndices[] =
{
	0, 1, 2,
};

static int kernelSize = 3;
static float kernelStdDev = 1.0f;
static float kernelCoeff[16];
static float currentTime;

#define RAW_COLOR 0
#define RAW_EMISSION 1
#define INTERMEDIATE_BLOOM 2
#define COMPLETE_BLOOM 3
#define MERGED 4
static int renderingMode = RAW_COLOR;

static void ComputeKernelCoefficients();

DissolveBloom2DTest::DissolveBloom2DTest(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height), m_EmissionThreshold(0.03f), m_EmissionColor({ 1.0f, 1.0f, 1.0f }), m_EmissionIntensity(1.0f), m_Paused(false),
	m_RawFramebuffer(width, height), m_IntermediateBloomFramebuffer(width, height), m_CompleteBloomFramebuffer(width, height), m_MergedFramebuffer(width, height),
	m_BoxVBO(boxVertices, sizeof(boxVertices)), m_FullscreenVBO(fullscreenVertices, sizeof(fullscreenVertices)),
	m_BoxIBO(boxIndices, sizeof(boxIndices) / sizeof(unsigned int)), m_FullscreenIBO(fullscreenIndices, sizeof(fullscreenIndices) / sizeof(unsigned int))
{
	core::GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	layout.Push(GL_FLOAT, 2);
	m_BoxVBO.SetVertexBufferLayout(layout);
	m_FullscreenVBO.SetVertexBufferLayout(layout);

	m_BoxVAO.AddVertexBuffer(m_BoxVBO);
	m_FullscreenVAO.AddVertexBuffer(m_FullscreenVBO);

	m_BoxTexture.LoadFromFile("../res/textures/Test.png");

	m_DissolveShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/DissolveBloom2DTest/dissolve2d_vs.glsl");
	m_DissolveShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/DissolveBloom2DTest/dissolve2d_fs.glsl");
	m_DissolveShader.CompileShaders();
	m_DissolveShader.SetUniformMat4("u_MVP", core::mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));
	
	m_BloomShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/DissolveBloom2DTest/bloom_vs.glsl");
	m_BloomShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/DissolveBloom2DTest/bloom_fs.glsl");
	m_BloomShader.CompileShaders();

	m_MergeShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/DissolveBloom2DTest/merge_vs.glsl");
	m_MergeShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/DissolveBloom2DTest/merge_fs.glsl");
	m_MergeShader.CompileShaders();

	m_FullscreenShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/DissolveBloom2DTest/fullscreen_vs.glsl");
	m_FullscreenShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/DissolveBloom2DTest/fullscreen_fs.glsl");
	m_FullscreenShader.CompileShaders();

	m_RawFramebuffer.AttachTexture(m_RawFramebufferColorTexture, GL_RGBA32F, GL_FLOAT, 0);
	m_RawFramebuffer.AttachTexture(m_RawFramebufferBloomTexture, GL_RGBA32F, GL_FLOAT, 1);
	m_RawFramebuffer.SetDrawBufferAttachments(2);
	m_RawFramebuffer.GenerateDepthStencilRenderbuffer();
	m_RawFramebuffer.EnableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_RawFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_IntermediateBloomFramebuffer.AttachTexture(m_IntermediateBloomFramebufferTexture, GL_RGBA32F, GL_FLOAT, 0);
	m_IntermediateBloomFramebuffer.SetDrawBufferAttachments(1);
	m_IntermediateBloomFramebuffer.GenerateDepthStencilRenderbuffer();
	m_IntermediateBloomFramebuffer.EnableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_IntermediateBloomFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_CompleteBloomFramebuffer.AttachTexture(m_CompleteBloomFramebufferTexture, GL_RGBA32F, GL_FLOAT, 0);
	m_CompleteBloomFramebuffer.SetDrawBufferAttachments(1);
	m_CompleteBloomFramebuffer.GenerateDepthStencilRenderbuffer();
	m_CompleteBloomFramebuffer.EnableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_CompleteBloomFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_MergedFramebuffer.AttachTexture(m_MergedFramebufferTexture, GL_RGBA32F, GL_FLOAT, 0);
	m_MergedFramebuffer.SetDrawBufferAttachments(1);
	m_MergedFramebuffer.GenerateDepthStencilRenderbuffer();
	m_MergedFramebuffer.EnableBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	m_MergedFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	core::GLFramebuffer::SetDefaultFramebuffer();
	core::GLFramebuffer::EnableDefaultBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	currentTime = -0.2f;

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
}

DissolveBloom2DTest::~DissolveBloom2DTest()
{
}


void DissolveBloom2DTest::Update(float deltaTime)
{
	if (!m_Paused)
		currentTime += deltaTime;
	m_DissolveShader.SetUniform1f("u_DissolveThreshold", 0.5f + 0.6f * sin(2.0f * currentTime));
	m_DissolveShader.SetUniform1f("u_EmissionThreshold", 0.5f + 0.6f * sin(2.0f * currentTime) + m_EmissionThreshold);
}

void DissolveBloom2DTest::Render()
{
	m_EmissionColor.a = 1.0f;
	m_DissolveShader.SetUniform4f("u_EmissionColor", m_EmissionColor);
	m_DissolveShader.SetUniform1f("u_EmissionIntensity", m_EmissionIntensity);

	m_RawFramebuffer.Bind();
	m_RawFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_DissolveShader.Bind();
	m_BoxVAO.Bind();
	m_BoxIBO.Bind();
	m_BoxTexture.Bind(0);
	m_DissolveTexture.Bind(1);
	glDrawElements(GL_TRIANGLES, m_BoxIBO.Count(), GL_UNSIGNED_INT, 0);

	ComputeKernelCoefficients();
	m_BloomShader.Bind();
	m_BloomShader.SetUniform1i("u_KernelSize", kernelSize);
	m_BloomShader.SetUniform1fv("u_KernelCoeff", kernelCoeff, kernelSize + 1);

	m_IntermediateBloomFramebuffer.Bind();
	m_IntermediateBloomFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_BloomShader.SetUniform1i("u_Vertical", 0);
	m_BloomShader.SetUniform1f("u_TexCoordOffset", 1.0f / m_Width);
	m_FullscreenVAO.Bind();
	m_FullscreenIBO.Bind();
	m_RawFramebufferBloomTexture.Bind(0);
	glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);

	m_CompleteBloomFramebuffer.Bind();
	m_CompleteBloomFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_BloomShader.SetUniform1i("u_Vertical", 1);
	m_BloomShader.SetUniform1f("u_TexCoordOffset", 1.0f / m_Height);
	m_IntermediateBloomFramebufferTexture.Bind(0);
	glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);

	m_MergedFramebuffer.Bind();
	m_MergedFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_MergeShader.Bind();
	m_RawFramebufferColorTexture.Bind(0);
	m_CompleteBloomFramebufferTexture.Bind(1);
	glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);

	core::GLFramebuffer::SetDefaultFramebuffer();
	m_FullscreenShader.Bind();

	switch (renderingMode)
	{
	case RAW_COLOR:
		m_RawFramebufferColorTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
		break;

	case RAW_EMISSION:
		m_RawFramebufferBloomTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
		break;

	case INTERMEDIATE_BLOOM:
		m_IntermediateBloomFramebufferTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
		break;

	case COMPLETE_BLOOM:
		m_CompleteBloomFramebufferTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
		break;

	case MERGED:
		m_MergedFramebufferTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
		break;
	}
}

void DissolveBloom2DTest::ImGuiRender()
{
	//if (ImGui::Button("Reset time"))
		//currentTime = -0.2f;
	ImGui::Checkbox("Paused", &m_Paused);

	ImGui::SliderInt("Size of blurring kernel", &kernelSize, 1, 15);
	ImGui::SliderFloat("Blurring kernel standard deviation", &kernelStdDev, 0.01f, 10.0f, "%.4f", 2.0f);

	ImGui::SliderFloat("Emission threshold", &m_EmissionThreshold, 0.001f, 1.0f, "%.4f", 2.0f);
	ImGui::SliderFloat("Emission intensity", &m_EmissionIntensity, 0.1f, 10.0f, "%.3f", 2.0f);
	ImGui::ColorPicker4("Emission color", &m_EmissionColor.r);

	ImGui::Separator();

	ImGui::RadioButton("Raw color", (int*)&renderingMode, RAW_COLOR);
	ImGui::RadioButton("Raw emission", (int*)&renderingMode, RAW_EMISSION);
	ImGui::RadioButton("Intermediate bloom", (int*)&renderingMode, INTERMEDIATE_BLOOM);
	ImGui::RadioButton("Complete bloom", (int*)&renderingMode, COMPLETE_BLOOM);
	ImGui::RadioButton("Merged", (int*)&renderingMode, MERGED);
}


void ComputeKernelCoefficients()
{
	float sum = -1.0f;
	float denominator = 0.5f / (kernelStdDev * kernelStdDev);
	float coeff;
	for (int i = 0; i <= kernelSize; i++)
	{
		coeff = std::exp(-i * i * denominator);
		kernelCoeff[i] = coeff;
		sum += 2.0f * coeff;
	}
	coeff = 1.0f / sum;
	for (int i = 0; i <= kernelSize; i++)
		kernelCoeff[i] *= coeff;
}