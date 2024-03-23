#include "GaussianBlurTest.h"

#include <Core.h>

#include <imgui.h>

float boxVertices[] =
{
	-0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.0f, 1.0f,
};

unsigned int boxIndices[] =
{
	0, 1, 2,
	0, 2, 3,
};

float fullscreenVertices[] =
{
	-1.0f, -1.0f,  0.0f, 0.0f,
	 3.0f, -1.0f,  2.0f, 0.0f,
	-1.0f,  3.0f,  0.0f, 2.0f,
};

unsigned int fullscreenIndices[] =
{
	0, 1, 2,
};

bool useGaussianBlur = false;
int kernelSize = 3;
float kernelStdDev = 1.0f;
float kernelCoeff[16];

void ComputeKernelCoefficients();

GaussianBlurTest::GaussianBlurTest(unsigned int width, unsigned int height)
	: m_Width(width), m_Height(height),
	m_NonBlurredFramebuffer(width, height),
	m_BoxVBO(boxVertices, sizeof(boxVertices)),
	m_IntermediateBlurredFramebuffer(width, height),
	m_FullscreenVBO(fullscreenVertices, sizeof(fullscreenVertices)),
	m_BoxIBO(boxIndices, sizeof(boxIndices) / sizeof(unsigned int)),
	m_FullscreenIBO(fullscreenIndices, sizeof(fullscreenIndices) / sizeof(unsigned int))
{
	core::GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	layout.Push(GL_FLOAT, 2);
	m_BoxVBO.SetVertexBufferLayout(layout);
	m_FullscreenVBO.SetVertexBufferLayout(layout);

	m_BoxVAO.AddVertexBuffer(m_BoxVBO);
	m_FullscreenVAO.AddVertexBuffer(m_FullscreenVBO);

	m_BoxTexture.LoadFromFile("../res/textures/Test.png");

	m_Shader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/GaussianBlurTest/simple_vs.glsl");
	m_Shader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/GaussianBlurTest/simple_fs.glsl");
	m_Shader.CompileShaders();
	m_Shader.SetUniformMat4("u_MVP", core::mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));

	m_BlurShader.AddShaderFromFile(GL_VERTEX_SHADER, "src/tests/GaussianBlurTest/gaussian_blur_1d_vs.glsl");
	m_BlurShader.AddShaderFromFile(GL_FRAGMENT_SHADER, "src/tests/GaussianBlurTest/gaussian_blur_1d_fs.glsl");
	m_BlurShader.CompileShaders();

	m_NonBlurredFramebuffer.AttachTexture(m_NonBlurredTexture, GL_RGB32F, 0);
	m_NonBlurredFramebuffer.SetDrawBufferAttachments(1);
	m_NonBlurredFramebuffer.GenerateDepthStencilRenderbuffer();
	m_NonBlurredFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	m_IntermediateBlurredFramebuffer.AttachTexture(m_IntermediateBlurredTexture, GL_RGB32F, 0);
	m_IntermediateBlurredFramebuffer.SetDrawBufferAttachments(1);
	m_IntermediateBlurredFramebuffer.GenerateDepthStencilRenderbuffer();
	m_IntermediateBlurredFramebuffer.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	memset(kernelCoeff, 0, sizeof(kernelCoeff));
}

GaussianBlurTest::~GaussianBlurTest()
{
}


void GaussianBlurTest::Update(float deltaTime)
{

}

void GaussianBlurTest::Render()
{
	if (useGaussianBlur)
	{
		m_NonBlurredFramebuffer.Bind();
		m_NonBlurredFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_Shader.Bind();
		m_BoxVAO.Bind();
		m_BoxIBO.Bind();
		m_BoxTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_BoxIBO.Count(), GL_UNSIGNED_INT, 0);

		ComputeKernelCoefficients();
		m_BlurShader.Bind();
		m_BlurShader.SetUniform1i("u_KernelSize", kernelSize);
		m_BlurShader.SetUniform1fv("u_KernelCoeff", kernelCoeff, kernelSize + 1);

		m_IntermediateBlurredFramebuffer.Bind();
		m_IntermediateBlurredFramebuffer.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_BlurShader.SetUniform1i("u_Vertical", 0);
		m_BlurShader.SetUniform1f("u_TexCoordOffset", 1.0f / m_Width);
		m_FullscreenVAO.Bind();
		m_FullscreenIBO.Bind();
		m_NonBlurredTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);

		core::GLFramebuffer::SetDefaultFramebuffer();
		m_BlurShader.SetUniform1i("u_Vertical", 1);
		m_BlurShader.SetUniform1f("u_TexCoordOffset", 1.0f / m_Height);
		m_IntermediateBlurredTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_FullscreenIBO.Count(), GL_UNSIGNED_INT, 0);
	}
	else
	{
		core::GLFramebuffer::SetDefaultFramebuffer();
		m_Shader.Bind();
		m_BoxVAO.Bind();
		m_BoxIBO.Bind();
		m_BoxTexture.Bind(0);
		glDrawElements(GL_TRIANGLES, m_BoxIBO.Count(), GL_UNSIGNED_INT, 0);
	}
}

void GaussianBlurTest::ImGuiRender()
{
	ImGui::Checkbox("Use Gaussian blur", &useGaussianBlur);
	ImGui::SliderInt("Size of blurring kernel", &kernelSize, 1, 15);
	ImGui::SliderFloat("Blurring kernel standard deviation", &kernelStdDev, 0.01f, 10.0f, "%.4f", 2.0f);
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