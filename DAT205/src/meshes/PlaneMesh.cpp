#include "PlaneMesh.h"

static float vertices[] =
{
	-0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, 0.0f, -0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f, 0.0f,  0.5f,  0.0f, 1.0f, 0.0f,
};

static unsigned int indices[] =
{
	0, 1, 2,
	0, 2, 3,
};

PlaneMesh::PlaneMesh(const vec3& position /*= { 0.0f, 0.0f, 0.0f }*/, const vec2& scale /*= { 0.0f, 0.0f }*/)
	: m_Position(position), m_Scale(scale.x, 1.0, scale.y)
{
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 3);
	layout.Push(GL_FLOAT, 3);

	m_VBO = std::make_shared<GLVertexBuffer>(vertices, sizeof(vertices));
	m_VBO->SetVertexBufferLayout(layout);

	m_VAO = std::make_shared<GLVertexArray>();
	m_VAO->AddVertexBuffer(*m_VBO);

	m_IBO = std::make_shared<GLIndexBuffer>(indices, sizeof(indices) / sizeof(unsigned int));

	m_Material.albedo = vec4(0.4f, 0.4f, 0.4f, 1.0f);
	m_Material.emission = 0.0f;
	m_Material.reflectivity = 0.5f;
	m_Material.shininess = 10.0f;
	m_Material.fresnel = 0.5f;
	m_Material.metalness = 0.5f;
}

PlaneMesh::~PlaneMesh()
{
}


void PlaneMesh::Render(const Renderer& renderer, GLShader& shader) const
{
	shader.Bind();
	mat4 M = mat4::Translate(m_Position) * mat4::Scale(m_Scale);
	mat4 MV = renderer.camera.GetViewMatrix() * M;
	const mat4& P = renderer.camera.projectionMatrix;
	shader.SetUniformMat4("u_MV", MV);
	shader.SetUniformMat4("u_MV_normal", mat4::Transpose(mat4::Inverse(MV)));
	shader.SetUniformMat4("u_MVP", P * MV);

	m_Material.Bind(shader);

	m_VAO->Bind();
	m_IBO->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->Count(), GL_UNSIGNED_INT, 0));
}