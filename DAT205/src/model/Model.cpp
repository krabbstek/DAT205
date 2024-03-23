#include "Model.h"

#include "Globals.h"
#include "tiny_obj_loader.h"
#include "graphics/Material.h"

#include <iostream>
#include <filesystem>

Model::Model(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader)
	: Renderable(prepassShader, mainShader), modelMatrix(1.0f)
{
}


std::shared_ptr<Model> Model::LoadModelFromOBJ(const char* file, std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader)
{
	std::filesystem::path filepath(file);
	std::string directory = filepath.parent_path().string();

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file, directory.c_str(), true);
	if (!err.empty())
	{
		std::cout << "Error: " << err << std::endl;
		return nullptr;
	}
	if (!warn.empty())
		std::cout << "Warning: " << warn << std::endl;

	std::shared_ptr<Model> model = std::make_shared<Model>(prepassShader, mainShader);

	// Read materials
	for (const tinyobj::material_t& m : materials) {
		Material material;
		material.name = m.name;
		material.albedo = vec4(m.diffuse[0], m.diffuse[1], m.diffuse[2], 1.0f);
		if (m.diffuse_texname != "") {
			material.albedoTexture = std::make_shared<GLTexture2D>();
			material.albedoTexture->LoadFromFile((directory + '/' + m.diffuse_texname).c_str());
			material.albedoTexture->SetMinMagFilter(GL_LINEAR);
			material.albedoTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.reflectivity = m.specular[0];
		if (m.specular_texname != "") {
			material.reflectivityTexture = std::make_shared<GLTexture2D>();
			material.reflectivityTexture->LoadFromFile((directory + '/' + m.specular_texname).c_str());
			material.reflectivityTexture->SetMinMagFilter(GL_LINEAR);
			material.reflectivityTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.metalness = m.metallic;
		if (m.metallic_texname != "") {
			material.metalnessTexture = std::make_shared<GLTexture2D>();
			material.metalnessTexture->LoadFromFile((directory + '/' + m.metallic_texname).c_str());
			material.metalnessTexture->SetMinMagFilter(GL_LINEAR);
			material.metalnessTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.fresnel = m.sheen;
		if (m.sheen_texname != "") {
			material.fresnelTexture = std::make_shared<GLTexture2D>();
			material.fresnelTexture->LoadFromFile((directory + '/' + m.sheen_texname).c_str());
			material.fresnelTexture->SetMinMagFilter(GL_LINEAR);
			material.fresnelTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.shininess = m.roughness;
		if (m.roughness_texname != "") {
			material.shininessTexture = std::make_shared<GLTexture2D>();
			material.shininessTexture->LoadFromFile((directory + '/' + m.roughness_texname).c_str());
			material.shininessTexture->SetMinMagFilter(GL_LINEAR);
			material.shininessTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.emission = m.emission[0];
		if (m.emissive_texname != "") {
			material.emissionTexture = std::make_shared<GLTexture2D>();
			material.emissionTexture->LoadFromFile((directory + '/' + m.emissive_texname).c_str());
			material.emissionTexture->SetMinMagFilter(GL_LINEAR);
			material.emissionTexture->SetWrapST(GL_CLAMP_TO_EDGE);
		}
		material.transparency = m.transmittance[0];
		model->m_Materials.push_back(material);
	}

	// Auto generate normals for faces which do not have normals
	std::vector<vec4> autoNormals(attrib.vertices.size() / 3);
	for (const auto& shape : shapes) {
		for (int face = 0; face < int(shape.mesh.indices.size()) / 3; face++) {
			vec3 v0 = vec3(
				attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 0].vertex_index * 3 + 2]
			);
			vec3 v1 = vec3(
				attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 1].vertex_index * 3 + 2]
			);
			vec3 v2 = vec3(
				attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 0],
				attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 1],
				attrib.vertices[shape.mesh.indices[face * 3 + 2].vertex_index * 3 + 2]
			);

			vec3 e0 = vec3::Normalize(v1 - v0);
			vec3 e1 = vec3::Normalize(v2 - v0);
			vec3 faceNormal = vec3::Normalize(vec3::Cross(e0, e1));

			autoNormals[shape.mesh.indices[face * 3 + 0].vertex_index] += vec4(faceNormal.x, faceNormal.y, faceNormal.z, 1.0f);
			autoNormals[shape.mesh.indices[face * 3 + 1].vertex_index] += vec4(faceNormal.x, faceNormal.y, faceNormal.z, 1.0f);
			autoNormals[shape.mesh.indices[face * 3 + 2].vertex_index] += vec4(faceNormal.x, faceNormal.y, faceNormal.z, 1.0f);
		}
	}
	for (vec4& normal : autoNormals)
		normal = (1.0f / normal.w) * normal;

	size_t numVertices = 0;
	for (tinyobj::shape_t& shape : shapes)
		numVertices += shape.mesh.indices.size();

	std::vector<vec3> positions;
	std::vector<vec3> normals;
	std::vector<vec2> texCoords;
	positions.resize(numVertices);
	normals.resize(numVertices);
	texCoords.resize(numVertices);

	// Create meshes from shapes. If a shape has different materials, it will be split into multiple meshes.
	int verticesSoFar = 0;
	for (const tinyobj::shape_t& shape : shapes)
	{
		// Split shape into multiple meshes if it has multiple materials.
		int nextMaterialIndex = shape.mesh.material_ids[0];
		int nextMaterialStartingFace = 0;
		std::vector<bool> finishedMaterials(materials.size(), false);
		int numberOfMaterialsInShape = 0;
		while (nextMaterialIndex != -1)
		{
			int currentMaterialIndex = nextMaterialIndex;
			int currentMaterialStartingFace = nextMaterialStartingFace;
			nextMaterialIndex = -1;
			nextMaterialStartingFace = -1;
			// Process a new Mesh with a unique material
			Mesh mesh;
			mesh.name = shape.name + "_" + materials[currentMaterialIndex].name;
			mesh.materialIndex = currentMaterialIndex;
			mesh.startIndex = verticesSoFar;
			numberOfMaterialsInShape += 1;

			int numberOfFaces = shape.mesh.indices.size() / 3;
			for (int i = currentMaterialStartingFace; i < numberOfFaces; i++)
			{
				if (shape.mesh.material_ids[i] != currentMaterialIndex)
				{
					if (nextMaterialIndex >= 0)
						continue;
					else if (finishedMaterials[shape.mesh.material_ids[i]])
						continue;
					else
					{ // Found a new material that we have not processed.
						nextMaterialIndex = shape.mesh.material_ids[i];
						nextMaterialStartingFace = i;
					}
				}
				else
				{
					// Generate vertices
					for (int j = 0; j < 3; j++) {
						int v = shape.mesh.indices[i * 3 + j].vertex_index;
						positions[verticesSoFar + j] = vec3(
							attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 0],
							attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 1],
							attrib.vertices[shape.mesh.indices[i * 3 + j].vertex_index * 3 + 2]);
						if (shape.mesh.indices[i * 3 + j].normal_index == -1)
							// No normal, use the autogenerated
							normals[verticesSoFar + j] = autoNormals[shape.mesh.indices[i * 3 + j].vertex_index];
						else
							normals[verticesSoFar + j] = vec3(
								attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 0],
								attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 1],
								attrib.normals[shape.mesh.indices[i * 3 + j].normal_index * 3 + 2]);
						if (shape.mesh.indices[i * 3 + j].texcoord_index == -1)
							// No UV coordinates. Use null. 
							texCoords[verticesSoFar + j] = vec2(0.0f);
						else
							texCoords[verticesSoFar + j] = vec2(
								attrib.texcoords[shape.mesh.indices[i * 3 + j].texcoord_index * 2 + 0],
								attrib.texcoords[shape.mesh.indices[i * 3 + j].texcoord_index * 2 + 1]);
					}
					verticesSoFar += 3;
				}
			}

			// Finalize and push this mesh to the list
			mesh.vertexCount = verticesSoFar - mesh.startIndex;
			model->m_Meshes.emplace_back(mesh);
			finishedMaterials[currentMaterialIndex] = true;
		}
		if (numberOfMaterialsInShape == 1)
			model->m_Meshes.back().name = shape.name;
	}

	// Fill VBOs with data
	model->m_VBOs.reserve(3);
	model->m_VBOs.emplace_back(positions.data(), positions.size() * sizeof(vec3));
	model->m_VBOs.emplace_back(normals.data(), normals.size() * sizeof(vec3));
	model->m_VBOs.emplace_back(texCoords.data(), texCoords.size() * sizeof(vec2));

	GLVertexBufferLayout positionLayout;
	GLVertexBufferLayout normalLayout;
	GLVertexBufferLayout texCoordsLayout;
	positionLayout.Push(GL_FLOAT, 3);
	normalLayout.Push(GL_FLOAT, 3);
	texCoordsLayout.Push(GL_FLOAT, 2);
	model->m_VBOs[0].SetVertexBufferLayout(positionLayout);
	model->m_VBOs[1].SetVertexBufferLayout(normalLayout);
	model->m_VBOs[2].SetVertexBufferLayout(texCoordsLayout);

	// Add VBOs to VAO
	model->m_VAO.AddVertexBuffer(model->m_VBOs[0]);
	model->m_VAO.AddVertexBuffer(model->m_VBOs[1]);
	model->m_VAO.AddVertexBuffer(model->m_VBOs[2]);

	return model;
}


void Model::PushMesh(const Mesh& mesh)
{
	m_Meshes.emplace_back(mesh);
}


void Model::PrepassRender(const Renderer& renderer)
{
	mat4 MV = renderer.camera.GetViewMatrix() * modelMatrix;
	mat4 prevMV = renderer.camera.GetPreviousViewMatrix() * prevModelMatrix;

	m_PrepassShader->Bind();
	m_PrepassShader->SetUniformMat4("u_MV", MV);
	m_PrepassShader->SetUniformMat4("u_MV_normal", mat4::Transpose(mat4::Inverse(MV)));
	m_PrepassShader->SetUniformMat4("u_MVP", renderer.camera.projectionMatrix * MV);
	m_PrepassShader->SetUniformMat4("u_PrevMV", prevMV);
	m_PrepassShader->SetUniformMat4("u_PrevMVP", renderer.camera.projectionMatrix * prevMV);

	m_VAO.Bind();

	for (const Mesh& mesh : m_Meshes)
		GLCall(glDrawArrays(GL_TRIANGLES, mesh.startIndex, (GLsizei)mesh.vertexCount));
}

void Model::Render(const Renderer& renderer)
{
	mat4 MV = renderer.camera.GetViewMatrix() * modelMatrix;
	mat4 prevMV = renderer.camera.GetPreviousViewMatrix() * prevModelMatrix;

	m_MainShader->Bind();
	m_MainShader->SetUniformMat4("u_MV", MV);
	m_MainShader->SetUniformMat4("u_MV_normal", mat4::Transpose(mat4::Inverse(MV)));
	m_MainShader->SetUniformMat4("u_MVP", renderer.camera.projectionMatrix * MV);
	m_MainShader->SetUniformMat4("u_PrevMV", prevMV);
	m_MainShader->SetUniformMat4("u_PrevMVP", renderer.camera.projectionMatrix * prevMV);
	m_MainShader->SetUniform1i("u_UseNormalMap", g_UseNormalMap);

	m_VAO.Bind();

	for (const Mesh& mesh : m_Meshes)
	{
		if (mesh.materialIndex >= 0)
			m_Materials[mesh.materialIndex].Bind(*m_MainShader);

		if (g_UseNormalMap && mesh.normalMap)
		{
			mesh.normalMap->Bind(6);
			m_MainShader->SetUniform1i("u_UseNormalMap", 1);
		}
		else
			m_MainShader->SetUniform1i("u_UseNormalMap", 0);

		GLCall(glDrawArrays(GL_TRIANGLES, mesh.startIndex, (GLsizei)mesh.vertexCount));
	}
}

