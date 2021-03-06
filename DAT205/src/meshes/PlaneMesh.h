#pragma once

#include "graphics/Renderable.h"

#include "graphics/Material.h"
#include "graphics/opengl/OpenGL.h"

class PlaneMesh : public Renderable
{
public:
	PlaneMesh(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader, const vec3& position = { 0.0f, 0.0f, 0.0f }, const vec2& scale = { 1.0f, 1.0f });
	~PlaneMesh();

	void PrepassRender(const Renderer& renderer) override;
	void Render(const Renderer& renderer) override;

private:
	std::shared_ptr<GLVertexArray> m_VAO;
	std::shared_ptr<GLVertexBuffer> m_VBO;
	std::shared_ptr<GLIndexBuffer> m_IBO;
	vec3 m_Position;
	vec3 m_Scale;
	Material m_Material;
};