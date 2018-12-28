#include <iostream>

#include "../Core.h"

using namespace core;

GLIndexBuffer* ibo;
GLVertexBuffer* vbo;
GLVertexArray* vao;
GLShader* shader;
GLTexture2D* texture;
GLTexture2D* colorOverlayTexture;
GLFramebuffer* velocityBuffer;

float t = 0;
mat4 prevMVP, MVP, projection = mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f);

void core::OnStart()
{
	//velocityBuffer = new GLFramebuffer(1280, 720, GL_RG16F, true);
	//velocityBuffer->Bind();

	MVP = projection;
	prevMVP = projection;

float v[] = 
	{
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.0f, 1.0f,
	};
	unsigned int i[] = 
	{
		0, 1, 2,
		0, 2, 3,
	};
	vbo = new GLVertexBuffer(v, sizeof(v));
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	layout.Push(GL_FLOAT, 2);
	vbo->SetVertexBufferLayout(layout);
	vao = new GLVertexArray(*vbo);
	vao->Bind();
	ibo = new GLIndexBuffer(i, sizeof(i) / sizeof(unsigned int));
	ibo->Bind();

	shader = new GLShader();
	shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_vert.glsl");
	shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_frag.glsl");
	shader->CompileShaders();
	shader->Bind();
	shader->SetUniformMat4("transformation", mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));

	texture = new GLTexture2D();
	texture->LoadFromFile("../Core/res/textures/Test.png");
	texture->SetWrapST(GL_CLAMP_TO_EDGE);
	texture->SetMinMagFilter(GL_LINEAR);
	texture->Bind(0);

	float t[] =
	{
		1.0f, 1.0f, 0.0f, 1.0f,    1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f, 1.0f,
	};
	colorOverlayTexture = new GLTexture2D();
	colorOverlayTexture->Load(GL_RGBA32F, (unsigned char*)t, 2, 2, GL_RGBA, GL_FLOAT);
	colorOverlayTexture->SetWrapST(GL_CLAMP_TO_EDGE);
	colorOverlayTexture->SetMinMagFilter(GL_LINEAR);
	colorOverlayTexture->Bind(1);

	vao->Bind();
	ibo->Bind();
}

void core::OnUpdate(float deltaTime)
{
	t += deltaTime;

	float x = sin(t * PI);
	prevMVP = MVP;
	MVP = projection * mat4::Translate(x, 0.0f, 0.0f);

	//shader->SetUniformMat4("prevMVP", prevMVP);
	//shader->SetUniformMat4("MVP", MVP);
}

void core::OnRender()
{
	glDrawElements(GL_TRIANGLES, ibo->Count(), GL_UNSIGNED_INT, 0);
}