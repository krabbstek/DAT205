#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class MotionBlurPass : public RenderPass
{
public:
	MotionBlurPass(Renderer& renderer,
		std::shared_ptr<GLShader> maxTileVelocityShader,
		std::shared_ptr<GLShader> motionBlurShader,
		std::shared_ptr<GLTexture2D> inputTexture,
		std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture,
		std::shared_ptr<GLTexture2D> outputTexture);
	~MotionBlurPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_MaxTileVelocityFramebuffer[2];
	GLuint m_Framebuffer;
	std::shared_ptr<GLShader> m_MaxTileVelocityShader;
	std::shared_ptr<GLShader> m_MotionBlurShader;
	std::shared_ptr<GLTexture2D> m_InputTexture;
	std::shared_ptr<GLTexture2D> m_MaxTileVelocityTexture[2];
	std::shared_ptr<GLTexture2D> m_ClipSpaceVelocityTexture;
	FullscreenMesh m_FullscreenMesh;
};