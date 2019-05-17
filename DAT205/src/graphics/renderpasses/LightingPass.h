#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"

class LightingPass : public RenderPass
{
public:
	LightingPass(Renderer& renderer,
		std::shared_ptr<GLShader> lightingPassShader,
		GLuint sharedDepthbuffer,
		std::shared_ptr<GLTexture2D> colorTexture,
		std::shared_ptr<GLTexture2D> irradianceMap,
		std::shared_ptr<GLTexture2D> reflectionMap,
		std::shared_ptr<GLTexture2D> ssaoTexture,
		std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
		std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
		std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO);
	~LightingPass();

	virtual void Render(std::vector<Renderable*>& renderables) override;

	inline GLuint GetFramebuffer() const { return m_Framebuffer; }

private:
	std::shared_ptr<GLShader> m_LightingPassShader;
	std::shared_ptr<GLShaderStorageBuffer> m_LightSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_LightIndexSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_TileIndexSSBO;
	std::shared_ptr<GLTexture2D> m_IrradianceMap;
	std::shared_ptr<GLTexture2D> m_ReflectionMap;
	std::shared_ptr<GLTexture2D> m_SSAOTexture;
	GLuint m_Framebuffer;
};