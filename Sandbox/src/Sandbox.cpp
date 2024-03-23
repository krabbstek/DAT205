#include <iostream>

#include "../Core.h"

using namespace core;

GLVertexBuffer* vbo;
GLVertexArray* vao;
GLShader* shader;

void core::OnStart()
{
	float v[] = 
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};
	vbo = new GLVertexBuffer(v, sizeof(v));
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	vao = new GLVertexArray(*vbo, layout);

	shader = new GLShader();
	shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_vert.glsl");
	shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_frag.glsl");
	shader->CompileShaders();
	shader->Bind();
}

void core::OnUpdate()
{

}

void core::OnRender()
{
	vbo->Bind();
	vao->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 3);
}