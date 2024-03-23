#pragma once

#include "ParticleSystem.h"

class GlowingParticleSystem : public ParticleSystem
{
public:
	GlowingParticleSystem(int maxNumParticles, std::shared_ptr<GLShader> glowingParticleShader);

	void UpdateParticles(float deltaTime) override;

	void Render(const Renderer& renderer) override;
};