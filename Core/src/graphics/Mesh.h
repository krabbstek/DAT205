#pragma once

#include "Common.h"

#include "Material.h"
#include "Renderable.h"

#include "opengl/GLIndexBuffer.h"
#include "opengl/GLShader.h"
#include "opengl/GLTexture2D.h"
#include "opengl/GLVertexArray.h"

namespace core {

	class CORE_API Mesh : public Renderable
	{
	public:
		Mesh(GLVertexArray* vao, GLIndexBuffer* ibo, GLShader* shader);
		~Mesh();

		static Mesh* Cube();

		virtual void Render();

		inline void AddTexture(GLTexture2D* texture) { m_Textures.push_back(texture); }
		inline void SetMaterial(Material* material) { m_Material = material; }
		inline void SetShader(GLShader* shader) { m_Shader = shader; }

		inline GLIndexBuffer* GetIndexBuffer() const { return m_IBO; }
		inline GLVertexArray* GetVertexArray() const { return m_VAO; }

	protected:
		Material* m_Material;
		GLIndexBuffer* m_IBO;
		GLShader* m_Shader;
		GLVertexArray* m_VAO;
		std::vector<GLTexture2D*> m_Textures;
	};

}