#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class SSAOPass : public RenderPass
{
public:
	SSAOPass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLShader> bilateralBlurShader, std::shared_ptr<GLTexture2D> viewSpacePositionTexture, std::shared_ptr<GLTexture2D> viewSpaceNormalTexture, std::shared_ptr<GLTexture2D> ssaoTexture);
	~SSAOPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_SSAOFramebuffer;
	GLuint m_BlurFramebuffer1;
	GLuint m_BlurFramebuffer2;
	std::shared_ptr<GLShader> m_SSAOShader;
	std::shared_ptr<GLShader> m_BilateralBlurShader;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLTexture2D> m_ViewSpaceNormalTexture;
	std::shared_ptr<GLTexture2D> m_RandomAnglesTexture;
	std::shared_ptr<GLTexture2D> m_IntermediateTexture1;
	std::shared_ptr<GLTexture2D> m_IntermediateTexture2;
	FullscreenMesh m_FullscreenMesh;
};