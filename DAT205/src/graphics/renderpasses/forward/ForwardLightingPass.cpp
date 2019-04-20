#include "ForwardLightingPass.h"

#include "graphics/opengl/OpenGL.h"

ForwardLightingPass::ForwardLightingPass(Renderer& renderer, std::shared_ptr<GLShader> shader)
	: RenderPass(renderer, shader)
{
}


void ForwardLightingPass::Render(std::vector<Renderable*>& renderables)
{
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glViewport(0, 0, g_WindowWidth, g_WindowHeight));

	RenderPass::Render(renderables);
}