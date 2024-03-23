#pragma once

#include "graphics/Renderable.h"

#include "graphics/opengl/OpenGL.h"

class TessellationCube : public Renderable
{
public:
	vec3 position;

	TessellationCube(std::shared_ptr<GLShader> prepassShader, std::shared_ptr<GLShader> mainShader, const vec3& position = { 0.0f, 0.0f, 0.0f });
	~TessellationCube();

	void PrepassRender(const Renderer& renderer) override;
	void Render(const Renderer& renderer) override;

private:
	GLVertexArray* m_VAO;
	GLVertexBuffer* m_VBO;
	GLIndexBuffer* m_IBO;
	GLTexture2D m_DisplacementTexture;
};