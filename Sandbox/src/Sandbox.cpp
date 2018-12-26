#include <iostream>

#include "../Core.h"

using namespace core;

GLIndexBuffer* ibo;
GLVertexBuffer* vbo;
GLVertexArray* vao;
GLShader* shader;
GLTexture* texture;

void core::OnStart()
{
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
	vao = new GLVertexArray(*vbo, layout);
	vao->Bind();
	ibo = new GLIndexBuffer(i, sizeof(i) / sizeof(unsigned int));
	ibo->Bind();

	shader = new GLShader();
	shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_vert.glsl");
	shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_frag.glsl");
	shader->CompileShaders();
	shader->Bind();
	//shader->SetUniform4f("color", vec4(0.2f, 0.3f, 0.8f, 1.0f));
	//shader->SetUniformMat4("transformation", mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));

	texture = new GLTexture("../Core/res/textures/Test.jpg", GL_RGB);
	texture->Bind(0);

	vao->Bind();
	ibo->Bind();
}

void core::OnUpdate()
{

}

void core::OnRender()
{
	glDrawElements(GL_TRIANGLES, ibo->Count(), GL_UNSIGNED_INT, 0);
}