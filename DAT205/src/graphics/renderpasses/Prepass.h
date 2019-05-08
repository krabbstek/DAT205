#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"

class Prepass : public RenderPass
{
public:
	Prepass(Renderer& renderer, std::shared_ptr<GLTexture2D> viewSpacePositionTexture, std::shared_ptr<GLTexture2D> viewSpaceNormalTexture, std::shared_ptr<GLTexture2D> clipSpaceVelocityTexture);
	~Prepass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

	inline GLuint GetDepthbuffer() const { return m_SharedDepthbuffer; }

private:
	GLuint m_PrepassFramebuffer;
	GLuint m_SharedDepthbuffer;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLTexture2D> m_ViewSpaceNormalTexture;
	std::shared_ptr<GLTexture2D> m_ClipSpaceVelocityTexture;
};