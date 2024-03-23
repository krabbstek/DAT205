#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class BloomPass : public RenderPass
{
public:
	BloomPass(Renderer& renderer, std::shared_ptr<GLShader> bloomShader, std::shared_ptr<GLShader> blur1DShader, std::shared_ptr<GLTexture2D> lightingPassColorTexture, std::shared_ptr<GLTexture2D> bloomInputTexture, std::shared_ptr<GLTexture2D> bloomOutputTexture);
	~BloomPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_BlurFramebuffer[3];
	GLuint m_BlurIntermediateFramebuffer;
	GLuint m_OutputFramebuffer;
	std::shared_ptr<GLShader> m_BloomShader;
	std::shared_ptr<GLShader> m_Blur1DShader;
	std::shared_ptr<GLTexture2D> m_LightingPassColorTexture;
	std::shared_ptr<GLTexture2D> m_BloomInputTexture;
	std::shared_ptr<GLTexture2D> m_BloomIntermediateTexture;
	std::shared_ptr<GLTexture2D> m_BloomBlurredTexture[3];
	std::shared_ptr<GLTexture2D> m_BloomOutputTexture;
	FullscreenMesh m_FullscreenMesh;
};