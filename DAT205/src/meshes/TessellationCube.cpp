#include "TessellationCube.h"

#include "Globals.h"

#include "graphics/Material.h"

float cubeVertices[] =
{
	// Position | Tex coords

	// Left
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

	// Right
	 0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,   0.0f, 1.0f,

	 // Bottom
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f, 1.0f,

	// Top
	-0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,

	// Back
	-0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

	// Front
	-0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
};

/*unsigned int cubeIndices[] =
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

	// Back
	16, 17, 18,
	16, 18, 19,

	// Front
	20, 21, 22,
	20, 22, 23,
};*/

unsigned int cubeIndices[] =
{
	 0,  1,  2,  3,
	 4,  5,  6,  7,
	 8,  9, 10, 11,
	12, 13, 14, 15,
	16, 17, 18, 19,
	20, 21, 22, 23,
};

TessellationCube::TessellationCube(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader, const vec3& position /*= { 0.0f, 0.0f, 0.0f }*/)
	: Renderable(prepassShader, mainShader), position(position)
{
	m_VBO = new GLVertexBuffer(cubeVertices, sizeof(cubeVertices));
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 3);
	layout.Push(GL_FLOAT, 2);
	m_VBO->SetVertexBufferLayout(layout);

	m_VAO = new GLVertexArray();
	m_VAO->AddVertexBuffer(*m_VBO);
	
	m_IBO = new GLIndexBuffer(cubeIndices, sizeof(cubeIndices));

	float displacement[64 * 64];
	vec2 coord;
	for (int x = 0; x < 64; x++)
	{
		coord.x = (float(x) - 31.5f) / 63.0f;
		for (int y = 0; y < 64; y++)
		{
			coord.y = (float(y) - 31.5f) / 63.0f;

			displacement[x + y * 64] = sqrtf(0.75f) - sqrtf(vec2::Dot(coord, coord) + 0.25f);
		}
	}
	m_DisplacementTexture.Load(GL_R16F, displacement, 64, 64, GL_RED, GL_FLOAT);
	m_DisplacementTexture.SetMinMagFilter(GL_LINEAR);
	m_DisplacementTexture.SetWrapST(GL_CLAMP_TO_EDGE);

	Material material;
	material.albedo = vec4(1.0f);
	material.fresnel = 0.5f;
	material.metalness = 0.5f;
	material.reflectivity = 0.5f;
	material.shininess = 10.0f;
	material.Bind(*m_MainShader);
}

TessellationCube::~TessellationCube()
{
	delete m_VAO;
	delete m_VBO;
	delete m_IBO;
}


void TessellationCube::PrepassRender(const Renderer& renderer)
{
	m_PrepassShader->Bind();
	mat4 M = mat4::Translate(position);
	mat4 V = renderer.camera.GetViewMatrix();
	mat4 P = renderer.camera.projectionMatrix;
	mat4 MV = V * M;
	m_PrepassShader->SetUniformMat4("u_MV", MV);
	m_PrepassShader->SetUniformMat4("u_MVP", P * MV);
	m_PrepassShader->SetUniformMat4("u_MV_normal", mat4::Transpose(mat4::Inverse(MV)));
	m_PrepassShader->SetUniform3f("u_CameraPosition", renderer.camera.position);
	m_PrepassShader->SetUniformMat4("u_ViewMatrix", renderer.camera.GetViewMatrix());
	m_PrepassShader->SetUniformMat4("u_ViewInverse", renderer.camera.GetInverseViewMatrix());

	m_DisplacementTexture.Bind(4);

	m_VAO->Bind();
	m_IBO->Bind();
	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));
	GLCall(glDrawElements(GL_PATCHES, m_IBO->Count(), GL_UNSIGNED_INT, 0));
}

void TessellationCube::Render(const Renderer& renderer)
{
	m_MainShader->Bind();
	mat4 M = mat4::Translate(position);
	mat4 V = renderer.camera.GetViewMatrix();
	mat4 P = renderer.camera.projectionMatrix;
	mat4 MV = V * M;
	m_MainShader->SetUniformMat4("u_MV", MV);
	m_MainShader->SetUniformMat4("u_MVP", P * MV);
	m_MainShader->SetUniformMat4("u_MV_normal", mat4::Transpose(mat4::Inverse(MV)));
	m_MainShader->SetUniform3f("u_CameraPosition", renderer.camera.position);
	m_MainShader->SetUniform1f("u_EnvironmentMultiplier", g_EnvironmentMultiplier);
	m_MainShader->SetUniform1i("u_NumTileCols", g_NumTileCols);
	m_MainShader->SetUniform1i("u_TileSize", g_TileSize);
	m_MainShader->SetUniformMat4("u_ViewMatrix", renderer.camera.GetViewMatrix());
	m_MainShader->SetUniformMat4("u_ViewInverse", renderer.camera.GetInverseViewMatrix());

	m_DisplacementTexture.Bind(4);

	m_VAO->Bind();
	m_IBO->Bind();
	GLCall(glPatchParameteri(GL_PATCH_VERTICES, 4));
	GLCall(glDrawElements(GL_PATCHES, m_IBO->Count(), GL_UNSIGNED_INT, 0));
}