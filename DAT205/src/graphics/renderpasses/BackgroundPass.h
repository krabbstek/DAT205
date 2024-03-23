#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class BackgroundPass : public RenderPass
{
public:
	BackgroundPass(Renderer& renderer, std::shared_ptr<GLShader> shader, GLuint targetFramebuffer, std::shared_ptr<GLTexture2D> environmentMap);

	virtual void Render(std::vector<Renderable*>&) override;

private:
	GLuint m_TargetFramebuffer;
	FullscreenMesh m_FullscreenMesh;
	std::shared_ptr<GLShader> m_Shader;
	std::shared_ptr<GLTexture2D> m_EnvironmentMap;
};