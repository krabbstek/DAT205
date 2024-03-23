#pragma once

#include "graphics/RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class Blur2DPass : public RenderPass
{
public:
	Blur2DPass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> inputTexture, std::shared_ptr<GLTexture2D> outputTexture);
	~Blur2DPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_Framebuffer1;
	GLuint m_Framebuffer2;
	std::shared_ptr<GLTexture2D> m_InputTexture;
	std::shared_ptr<GLTexture2D> m_OutputTexture;
	std::shared_ptr<GLTexture2D> m_IntermediateTexture;
	FullscreenMesh m_FullscreenMesh;
};