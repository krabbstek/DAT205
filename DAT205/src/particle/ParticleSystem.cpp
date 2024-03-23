#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int maxNumParticles, std::shared_ptr<GLShader> particleShader)
	: Renderable(nullptr, particleShader), m_VBO(nullptr, maxNumParticles * sizeof(Particle)), m_ParticleShader(particleShader), m_Particles(new Particle[maxNumParticles]), m_VertexData(new VertexData[maxNumParticles]), m_NumParticles(0), m_MaxNumParticles(maxNumParticles)
{
	GLVertexBufferLayout layout;
	// Current view space position
	layout.Push(GL_FLOAT, 3);
	// Previous view space position
	layout.Push(GL_FLOAT, 3);
	// Particle scale
	layout.Push(GL_FLOAT, 1);
	// Emission
	layout.Push(GL_FLOAT, 3);
	// Current lifetime
	layout.Push(GL_FLOAT, 1);
	// Particle life length
	layout.Push(GL_FLOAT, 1);

	m_VBO.SetVertexBufferLayout(layout);

	m_VAO.AddVertexBuffer(m_VBO);
}

ParticleSystem::~ParticleSystem()
{
	delete[] m_Particles;
	delete[] m_VertexData;
}


void ParticleSystem::Render(const Renderer& renderer) const
{
	const mat4& V = renderer.camera.GetViewMatrix();
	const mat4& prevV = renderer.camera.GetPreviousViewMatrix();
	for (size_t i = 0; i < m_NumParticles; i++)
	{
		m_VertexData[i].currentPosition = V * m_Particles[i].position;
		m_VertexData[i].previousPosition = prevV * m_Particles[i].position;
		m_VertexData[i].scale = m_Particles[i].scale;
		m_VertexData[i].emission = m_Particles[i].emission;
		m_VertexData[i].lifeTime = m_Particles[i].lifeTime;
		m_VertexData[i].lifeTime = m_Particles[i].lifeLength;
	}

	m_VBO.Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumParticles * sizeof(VertexData), m_VertexData));

	m_VAO.Bind();

	for (size_t i = 0; i < m_Textures.size(); i++)
		m_Textures[i]->Bind(i);

	m_ParticleShader->Bind();
	m_ParticleShader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));
	m_ParticleShader->SetUniformMat4("u_P", renderer.camera.projectionMatrix);

	GLCall(glDrawArrays(GL_POINTS, 0, m_NumParticles));
}