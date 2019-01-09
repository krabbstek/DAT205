#include "pch.h"

#include "Mesh.h"

#include "opengl/GLVertexBuffer.h"

namespace core {

	Mesh::Mesh(GLVertexArray* vao, GLIndexBuffer* ibo, GLShader* shader)
		: m_VAO(vao), m_IBO(ibo), m_Shader(shader)
	{
	}


	Mesh* Mesh::Cube()
	{
		static Mesh* cube = nullptr;
		if (cube)
			return cube;

		static float v[] =
		{
			 // Pos | normals | uv

			 // Left
			-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
										  		 	    
			 // Right					  		 	    
			 0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
													    
			 // Bottom								    
			-0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

			 // Top
			-0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

			 // Front
			-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,

			 // Back
			 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		};
		GLVertexBufferLayout layout;
		layout.Push(GL_FLOAT, 3);
		layout.Push(GL_FLOAT, 3);
		layout.Push(GL_FLOAT, 2);
		static GLVertexBuffer vbo(v, sizeof(v));
		vbo.SetVertexBufferLayout(layout);

		static GLVertexArray vao = GLVertexArray();
		vao.AddVertexBuffer(vbo);

		static unsigned int i[] =
		{
			// Left
			0,  1,  2,
			0,  2,  3,

			// Right
			4,  5,  6,
			4,  6,  7,

			// Bottom
			8,  9, 10,
			8, 10, 11,

			// Top
			12, 13, 14,
			12, 14, 15,

			// Front
			16, 17, 18,
			16, 18, 19,

			// Back
			20, 21, 22,
			20, 22, 23,
		};
		static GLIndexBuffer ibo = GLIndexBuffer(i, sizeof(i) / sizeof(unsigned int));

		cube = new Mesh(&vao, &ibo, nullptr);

		return cube;
	}

}