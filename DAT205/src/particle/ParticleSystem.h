#pragma once

#include "graphics/Renderable.h"

#include "graphics/opengl/OpenGL.h"

struct Particle
{
	vec3 position;
	vec3 velocity;
	float scale;
	vec3 emission;
	float lifetime;
	float lifelength;
};

struct VertexData
{
	vec3 currentPosition;
	vec3 previousPosition;
	float scale;
	vec3 emission;
	float lifetime;
	float lifelength;
};

class ParticleSystem : public Renderable
{
public:
	ParticleSystem(int maxNumParticles, std::shared_ptr<GLShader> particleShader);
	virtual ~ParticleSystem();

	virtual inline void AddTexture(std::shared_ptr<GLTexture2D> texture)
	{
		if (texture)
			m_Textures.emplace_back(texture);
	}

	virtual void SpawnParticle(const Particle& particle);
	virtual void UpdateParticles(float deltaTime) = 0;

	inline Particle& GetParticle(int i) { return m_Particles[i]; }

	virtual void PrepassRender(const Renderer& renderer) override {}
	virtual void Render(const Renderer& renderer) override;

	inline bool IsTransparent() const override { return true; }

	inline int NumParticles() const { return m_NumParticles; }
	inline int MaxNumParticles() const { return m_MaxNumParticles; }

protected:
	Particle* m_Particles;
	VertexData* m_VertexData;
	int m_NumParticles;
	int m_MaxNumParticles;
	GLVertexArray m_VAO;
	GLVertexBuffer m_VBO;
	std::shared_ptr<GLShader> m_ParticleShader;
	std::vector<std::shared_ptr<GLTexture2D>> m_Textures;
};