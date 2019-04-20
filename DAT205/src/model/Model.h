#pragma once

#include "graphics/Material.h"
#include "graphics/Renderable.h"
#include "graphics/opengl/OpenGL.h"

struct Mesh
{
	std::string name;
	unsigned int startIndex;	// Start index of index buffer
	unsigned int vertexCount;	// Number of vertices to 
	unsigned int materialIndex; // Index of material inside Model
};

class Model : public Renderable
{
public:
	Model(const float* data, const unsigned int dataSize, const GLVertexBufferLayout vboLayout);

	static Model* LoadModelFromOBJ(const char* file);

	void PushMesh(const Mesh& mesh);

	void Render(const Renderer& renderer, GLShader& shader) const override;

private:
	Model();

protected:
	GLVertexArray m_VAO;
	GLVertexBuffer m_VBO;
	std::vector<Material> m_Materials;
	std::vector<Mesh> m_Meshes;
};