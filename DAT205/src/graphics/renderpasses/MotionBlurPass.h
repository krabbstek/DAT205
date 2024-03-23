#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

class MotionBlurPass : public RenderPass
{
public:
	MotionBlurPass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> inputTexture, std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture, std::shared_ptr<GLTexture2D> outputTexture);
	~MotionBlurPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	GLuint m_Framebuffer;
	GLuint m_Depthbuffer;
	std::shared_ptr<GLShader> m_Shader;
	std::shared_ptr<GLTexture2D> m_InputTexture;
	std::shared_ptr<GLTexture2D> m_ClipSpaceVelocityTexture;
	FullscreenMesh m_FullscreenMesh;
};