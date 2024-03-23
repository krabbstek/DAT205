#pragma once

#include "graphics/Renderable.h"

#include "graphics/opengl/OpenGL.h"

class FullscreenMesh : public Renderable
{
public:
	FullscreenMesh(std::shared_ptr<GLShader> mainShader);
	FullscreenMesh(std::shared_ptr<GLShader> mainShader, std::shared_ptr<GLTexture2D> texture);
	FullscreenMesh(std::shared_ptr<GLShader> mainShader, std::vector<std::shared_ptr<GLTexture2D>> textures);
	~FullscreenMesh();

	void AddTexture(std::shared_ptr<GLTexture2D> texture);

	void PrepassRender(const Renderer& renderer) override {}
	void Render(const Renderer& renderer) override;

private:
	std::vector<std::shared_ptr<GLTexture2D>> m_Textures;
	std::shared_ptr<GLVertexArray> m_VAO;
	std::shared_ptr<GLVertexBuffer> m_VBO;
};