#include "GlowingParticleSystem.h"

#include "Globals.h"

#include <algorithm>

GlowingParticleSystem::GlowingParticleSystem(int maxNumParticles, std::shared_ptr<GLShader> glowingParticleShader)
	: ParticleSystem(maxNumParticles, glowingParticleShader)
{
}


void GlowingParticleSystem::UpdateParticles(float deltaTime)
{
	for (int i = 0; i < m_NumParticles; i++)
	{
		Particle& particle = m_Particles[i];
		particle.lifetime += deltaTime;
		// Kill particle if lifetime >= lifelength
		if (particle.lifetime >= particle.lifelength)
		{
			m_Particles[i--] = m_Particles[--m_NumParticles];
			continue;
		}

		particle.position += particle.velocity * deltaTime;
	}
}


void GlowingParticleSystem::Render(const Renderer& renderer)
{
	const mat4& V = renderer.camera.GetViewMatrix();
	const mat4& prevV = renderer.camera.GetPreviousViewMatrix();
	for (size_t i = 0; i < m_NumParticles; i++)
	{
		m_VertexData[i].currentPosition = V * m_Particles[i].position;
		m_VertexData[i].previousPosition = prevV * m_Particles[i].position;
		m_VertexData[i].scale = m_Particles[i].scale;
		m_VertexData[i].emission = m_Particles[i].emission * std::min(1.0f, 2.0f * (0.5f - abs(m_Particles[i].lifetime / m_Particles[i].lifelength - 0.5f)));
		m_VertexData[i].lifetime = m_Particles[i].lifetime;
		m_VertexData[i].lifelength = m_Particles[i].lifelength;
	}

	std::sort(m_VertexData, m_VertexData + m_NumParticles, [](const VertexData& a, const VertexData& b) { return a.currentPosition.z < b.currentPosition.z; });

	m_VBO.Bind();
	GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, m_NumParticles * sizeof(VertexData), m_VertexData));

	m_VAO.Bind();

	for (size_t i = 0; i < m_Textures.size(); i++)
		m_Textures[i]->Bind(i);

	m_ParticleShader->Bind();
	m_ParticleShader->SetUniform1f("u_LightIntensityMultiplier", g_GlowingParticleLightIntensityMultiplier);
	m_ParticleShader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));
	m_ParticleShader->SetUniformMat4("u_P", renderer.camera.projectionMatrix);

	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));
	GLCall(glEnable(GL_PROGRAM_POINT_SIZE));

	GLCall(glDrawArrays(GL_POINTS, 0, m_NumParticles));

	GLCall(glDisable(GL_BLEND));
}