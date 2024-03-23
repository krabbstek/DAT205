#include <iostream>

#include "../Core.h"

using namespace core;

GLVertexBuffer* vbo;
GLVertexArray* vao;

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