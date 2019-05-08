#include "FullscreenMesh.h"

static float vertices[] =
{
	-1.0f, -1.0f,
	 3.0f, -1.0f,
	-1.0f,  3.0f,
};

static unsigned int indices[] =
{
	0, 1, 2,
};

FullscreenMesh::FullscreenMesh(std::shared_ptr<GLShader> mainShader)
	: Renderable(std::shared_ptr<GLShader>(), mainShader)
{
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);

	m_VBO = std::make_shared<GLVertexBuffer>(vertices, sizeof(vertices));
	m_VBO->SetVertexBufferLayout(layout);

	m_VAO = std::make_shared<GLVertexArray>();
	m_VAO->AddVertexBuffer(*m_VBO);
}

FullscreenMesh::FullscreenMesh(std::shared_ptr<GLShader> mainShader, std::shared_ptr<GLTexture2D> texture)
	: Renderable(std::shared_ptr<GLShader>(), mainShader)
{
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);

	m_VBO = std::make_shared<GLVertexBuffer>(vertices, sizeof(vertices));
	m_VBO->SetVertexBufferLayout(layout);

	m_VAO = std::make_shared<GLVertexArray>();
	m_VAO->AddVertexBuffer(*m_VBO);

	m_Textures.push_back(texture);
}

FullscreenMesh::FullscreenMesh(std::shared_ptr<GLShader> mainShader, std::vector<std::shared_ptr<GLTexture2D>> textures)
	: Renderable(std::shared_ptr<GLShader>(), mainShader), m_Textures(textures)
{
	GLVertexBufferLayout layout;
	layout.Push(GL_FLOAT, 2);

	m_VBO = std::make_shared<GLVertexBuffer>(vertices, sizeof(vertices));
	m_VBO->SetVertexBufferLayout(layout);

	m_VAO = std::make_shared<GLVertexArray>();
	m_VAO->AddVertexBuffer(*m_VBO);
}

FullscreenMesh::~FullscreenMesh()
{
}


void FullscreenMesh::AddTexture(std::shared_ptr<GLTexture2D> texture)
{
	m_Textures.push_back(texture);
}


void FullscreenMesh::Render(const Renderer& renderer)
{
	m_MainShader->Bind();
	m_VAO->Bind();
	size_t size = m_Textures.size();
	for (unsigned int i = 0; i < size; i++)
		m_Textures[i]->Bind(i);
	GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
}