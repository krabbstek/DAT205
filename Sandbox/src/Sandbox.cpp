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
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 0.0f,  1.0f
	};
	vbo = new GLVertexBuffer(v, sizeof(v));
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);
	vao = new GLVertexArray(*vbo, layout);
	vao->Bind();

	shader = new GLShader();
	shader->AddShaderFromFile(GL_VERTEX_SHADER, "../Core/res/shaders/basic_vert.glsl");
	shader->AddShaderFromFile(GL_FRAGMENT_SHADER, "../Core/res/shaders/basic_frag.glsl");
	shader->CompileShaders();
	shader->Bind();
	shader->SetUniform4f("color", vec4(0.2f, 0.3f, 0.8f, 1.0f));
	shader->SetUniformMat4("transformation", mat4::Orthographic(-16.0f / 9.0f, 16.0f / 9.0f, -1.0f, 1.0f, 1.0f, -1.0f));
}

void core::OnUpdate()
{

}

void core::OnRender()
{
	glDrawArrays(GL_TRIANGLES, 0, 3);
}