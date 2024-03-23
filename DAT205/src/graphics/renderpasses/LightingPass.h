#pragma once

#include "graphics/RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"

class LightingPass : public RenderPass
{
public:
	LightingPass(
		Renderer& renderer, std::shared_ptr<GLShader> shader,
		std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
		std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
		std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO,
		std::shared_ptr<GLTexture2D> irradianceMap,
		std::shared_ptr<GLTexture2D> reflectionMap);

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	std::shared_ptr<GLShaderStorageBuffer> m_LightSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_LightIndexSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_TileIndexSSBO;
	std::shared_ptr<GLTexture2D> m_IrradianceMap;
	std::shared_ptr<GLTexture2D> m_ReflectionMap;
};