#pragma once

#include "graphics/RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"

class Prepass : public RenderPass
{
public:
	Prepass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> viewSpacePositionTexture, std::shared_ptr<GLTexture2D> viewSpaceNormalTexture);
	~Prepass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

	inline GLuint GetDepthbuffer() const { return m_SharedDepthbuffer; }

private:
	GLuint m_PrepassFramebuffer;
	GLuint m_SharedDepthbuffer;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLTexture2D> m_ViewSpaceNormalTexture;
};