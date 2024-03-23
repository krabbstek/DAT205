#include "pch.h"

#include "Model.h"

#include "math/vec2.h"
#include "math/vec3.h"

#include "system/file_utils.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#include <cstdlib>

struct IndexSet
{
	unsigned int positionIndex;
	unsigned int textureIndex;
	unsigned int normalIndex;

	inline bool operator==(const IndexSet& indexSet) const
	{
		return positionIndex == indexSet.positionIndex
			&& textureIndex == indexSet.textureIndex
			&& normalIndex == indexSet.normalIndex;
	}
};

template<>
struct std::hash<IndexSet>
{
	const size_t operator()(const IndexSet& indexSet) const
	{
		return indexSet.positionIndex | (indexSet.textureIndex << 22) | (indexSet.normalIndex << 43);
	}
};

namespace core {

	Assimp::Importer importer;

	Model::Model()
		: m_VAO(nullptr), m_IBO(nullptr)
	{
	}

	Model::~Model()
	{
		if (m_VAO)
			delete m_VAO;

		if (m_IBO)
			delete m_IBO;
	}


	Model* Model::LoadOBJModelFromFile(const char* filePath)
	{
		std::stringstream ss(ReadFullFile(filePath));
		std::string line;

		std::vector<vec3> vertexPositions;
		std::vector<vec2> vertexTexCoords;
		std::vector<vec3> vertexNormals;
		vec3 vec;
		vec2 uv;
		std::vector<float> vertices;
		unsigned int numVertices = 0;

		unsigned int positionIndex1;
		unsigned int positionIndex2;
		unsigned int positionIndex3;
		unsigned int textureIndex1;
		unsigned int textureIndex2;
		unsigned int textureIndex3;
		unsigned int normalIndex1;
		unsigned int normalIndex2;
		unsigned int normalIndex3;
		
		std::unordered_map<IndexSet, unsigned int> IndexMap;
		std::vector<unsigned int> indices;

		while (std::getline(ss, line))
		{
			// Skip comments
			if (line.find("#") != std::string::npos)
				continue;

			if (line.find("vt ") != std::string::npos)
			{
				int result = sscanf(line.c_str(), "vt %f %f", &uv.x, &uv.y);
				if (result == 2)
					vertexTexCoords.push_back(uv);
			}
			else if (line.find("vn ") != std::string::npos)
			{
				int result = sscanf(line.c_str(), "vn %f %f %f", &vec.x, &vec.y, &vec.z);
				if (result == 3)
					vertexNormals.push_back(vec3::Normalize(vec));
			}
			else if (line.find("v ") != std::string::npos)
			{
				int result = sscanf(line.c_str(), "v %f %f %f", &vec.x, &vec.y, &vec.z);
				if (result == 3)
					vertexPositions.push_back(vec);
			}
			else if (line.find("f ") != std::string::npos)
			{
				int result = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&positionIndex1, &textureIndex1, &normalIndex1,
					&positionIndex2, &textureIndex2, &normalIndex2,
					&positionIndex3, &textureIndex3, &normalIndex3);

				if (result == 9)
				{
					if (positionIndex1 < 0)
						positionIndex1 += vertexPositions.size();
					if (positionIndex2 < 0)
						positionIndex2 += vertexPositions.size();
					if (positionIndex3 < 0)
						positionIndex3 += vertexPositions.size();

					if (textureIndex1 < 0)
						textureIndex1 += vertexTexCoords.size();
					if (textureIndex2 < 0)
						textureIndex2 += vertexTexCoords.size();
					if (textureIndex3 < 0)
						textureIndex3 += vertexTexCoords.size();

					if (normalIndex1 < 0)
						normalIndex1 += vertexNormals.size();
					if (normalIndex2 < 0)
						normalIndex2 += vertexNormals.size();
					if (normalIndex3 < 0)
						normalIndex3 += vertexNormals.size();

					positionIndex1--;
					positionIndex2--;
					positionIndex3--;
					textureIndex1--;
					textureIndex2--;
					textureIndex3--;
					normalIndex1--;
					normalIndex2--;
					normalIndex3--;

					// IndexSet does not exist
					if (IndexMap.find({ positionIndex1, textureIndex1, normalIndex1 }) == IndexMap.end())
					{
						IndexMap[{ positionIndex1, textureIndex1, normalIndex1 }] = numVertices;
						indices.push_back(numVertices++);
						vertices.push_back(vertexPositions[positionIndex1].x);
						vertices.push_back(vertexPositions[positionIndex1].y);
						vertices.push_back(vertexPositions[positionIndex1].z);
						vertices.push_back(vertexTexCoords[textureIndex1].x);
						vertices.push_back(vertexTexCoords[textureIndex1].y);
						vertices.push_back(vertexNormals[normalIndex1].x);
						vertices.push_back(vertexNormals[normalIndex1].y);
						vertices.push_back(vertexNormals[normalIndex1].z);
					}
					// IndexSet already exists
					else
					{
						indices.push_back(IndexMap[{ positionIndex1, textureIndex1, normalIndex1 }]);
					}

					// IndexSet does not exist
					if (IndexMap.find({ positionIndex2, textureIndex2, normalIndex2 }) == IndexMap.end())
					{
						IndexMap[{ positionIndex2, textureIndex2, normalIndex2 }] = numVertices;
						indices.push_back(numVertices++);
						vertices.push_back(vertexPositions[positionIndex2].x);
						vertices.push_back(vertexPositions[positionIndex2].y);
						vertices.push_back(vertexPositions[positionIndex2].z);
						vertices.push_back(vertexTexCoords[textureIndex2].x);
						vertices.push_back(vertexTexCoords[textureIndex2].y);
						vertices.push_back(vertexNormals[normalIndex2].x);
						vertices.push_back(vertexNormals[normalIndex2].y);
						vertices.push_back(vertexNormals[normalIndex2].z);
					}
					// IndexSet already exists
					else
					{
						indices.push_back(IndexMap[{ positionIndex2, textureIndex2, normalIndex2 }]);
					}

					// IndexSet does not exist
					if (IndexMap.find({ positionIndex3, textureIndex3, normalIndex3 }) == IndexMap.end())
					{
						IndexMap[{ positionIndex3, textureIndex3, normalIndex3 }] = numVertices;
						indices.push_back(numVertices++);
						vertices.push_back(vertexPositions[positionIndex3].x);
						vertices.push_back(vertexPositions[positionIndex3].y);
						vertices.push_back(vertexPositions[positionIndex3].z);
						vertices.push_back(vertexTexCoords[textureIndex3].x);
						vertices.push_back(vertexTexCoords[textureIndex3].y);
						vertices.push_back(vertexNormals[normalIndex3].x);
						vertices.push_back(vertexNormals[normalIndex3].y);
						vertices.push_back(vertexNormals[normalIndex3].z);
					}
					// IndexSet already exists
					else
					{
						indices.push_back(IndexMap[{ positionIndex3, textureIndex3, normalIndex3 }]);
					}
				}
			}
		}

		float* v = new float[vertices.size()];
		memcpy(v, &vertices[0], vertices.size() * sizeof(float));
		unsigned int* i = new unsigned int[indices.size()];
		memcpy(i, &indices[0], indices.size() * sizeof(unsigned int));

		Model* model = new Model();

		GLVertexBuffer* vbo = new GLVertexBuffer(v, vertices.size() * sizeof(float));
		GLVertexBufferLayout layout;
		layout.Push(GL_FLOAT, 3);
		layout.Push(GL_FLOAT, 2);
		layout.Push(GL_FLOAT, 3);
		vbo->SetVertexBufferLayout(layout);

		GLVertexArray* vao = new GLVertexArray();
		vao->AddVertexBuffer(*vbo);

		GLIndexBuffer* ibo = new GLIndexBuffer(i, indices.size());

		model->m_VAO = vao;
		model->m_IBO = ibo;

		return model;
	}

}