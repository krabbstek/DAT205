#include "GlowingParticleSystem.h"

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

		particle.position += particle.velocity;
	}
}