#pragma once

#include "graphics/Renderable.h"

#include "graphics/opengl/OpenGL.h"

class Cube : public Renderable
{
public:
	vec3 position;

	Cube(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader, const vec3& position = { 0.0f, 0.0f, 0.0f });
	~Cube();

	void PrepassRender(const Renderer& renderer) override;
	void Render(const Renderer& renderer) override;

private:
	GLVertexArray* m_VAO;
	GLVertexBuffer* m_VBO;
	GLIndexBuffer* m_IBO;
};