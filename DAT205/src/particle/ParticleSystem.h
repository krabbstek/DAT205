#pragma once

#include "graphics/Renderable.h"

#include "graphics/opengl/OpenGL.h"

struct Particle
{
	vec3 position;
	vec3 velocity;
	float scale;
	vec3 emission;
	float lifeTime;
	float lifeLength;
};

struct VertexData
{
	vec3 currentPosition;
	vec3 previousPosition;
	float scale;
	vec3 emission;
	float lifeTime;
	float lifeLength;
};

class ParticleSystem : public Renderable
{
public:
	ParticleSystem(int maxNumParticles, std::shared_ptr<GLShader> particleShader);
	~ParticleSystem();

	inline void AddTexture(std::shared_ptr<GLTexture2D> texture)
	{
		if (texture)
			m_Textures.emplace_back(texture);
	}

	void PrepassRender(const Renderer& renderer) const override {}
	void Render(const Renderer& renderer) const override;

	inline bool IsTransparent() const override { return true; }

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