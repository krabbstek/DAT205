#pragma once

#include "graphics/Material.h"
#include "graphics/Renderable.h"
#include "graphics/opengl/OpenGL.h"

struct Mesh
{
	std::string name;
	int startIndex;	// Start index of index buffer
	int vertexCount;	// Number of vertices to 
	int materialIndex; // Index of material inside Model
	std::shared_ptr<GLTexture2D> normalMap;
};

class Model : public Renderable
{
public:
	mat4 modelMatrix;
	mat4 prevModelMatrix;

	Model(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader);

	static std::shared_ptr<Model> LoadModelFromOBJ(const char* file, std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader);

	inline void Update() { prevModelMatrix = modelMatrix; }

	void PushMesh(const Mesh& mesh);

	void PrepassRender(const Renderer& renderer) override;
	void Render(const Renderer& renderer) override;

	inline std::vector<Material>& GetMaterials() { return m_Materials; }
	inline std::vector<Mesh>& GetMeshes() { return m_Meshes; }

protected:
	GLVertexArray m_VAO;
	std::vector<GLVertexBuffer> m_VBOs;
	std::vector<Material> m_Materials;
	std::vector<Mesh> m_Meshes;
};