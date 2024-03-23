#pragma once

#include "Common.h"

#include "opengl/GLIndexBuffer.h"
#include "opengl/GLShader.h"
#include "opengl/GLVertexArray.h"

namespace core {

	class CORE_API Mesh
	{
	public:
		Mesh(GLVertexArray* vao, GLIndexBuffer* ibo, GLShader* shader);

		static Mesh* Cube();

		inline void SetShader(GLShader* shader) { m_Shader = shader; }

		inline GLIndexBuffer* GetIndexBuffer() const { return m_IBO; }
		inline GLVertexArray* GetVertexArray() const { return m_VAO; }

	protected:
		GLIndexBuffer* m_IBO;
		GLVertexArray* m_VAO;
		GLShader* m_Shader;
	};

}