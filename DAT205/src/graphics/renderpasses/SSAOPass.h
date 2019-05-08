#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class SSAOPass : public RenderPass
{
public:
	SSAOPass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> viewSpacePositionTexture, std::shared_ptr<GLTexture2D> viewSpaceNormalTexture, std::shared_ptr<GLTexture2D> ssaoTexture);
	~SSAOPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_Framebuffer;
	std::shared_ptr<GLShader> m_Shader;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLTexture2D> m_ViewSpaceNormalTexture;
	std::shared_ptr<GLTexture2D> m_RandomAnglesTexture;
	FullscreenMesh m_FullscreenMesh;
};