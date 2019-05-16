#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class BackgroundPrepass : public RenderPass
{
public:
	BackgroundPrepass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture);
	~BackgroundPrepass();

	virtual void Render(std::vector<Renderable*>&) override;

private:
	GLuint m_Framebuffer;
	FullscreenMesh m_FullscreenMesh;
	std::shared_ptr<GLShader> m_Shader;
};