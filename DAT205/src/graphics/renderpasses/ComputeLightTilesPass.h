#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"

class ComputeLightTilesPass : public RenderPass
{
public:
	ComputeLightTilesPass(Renderer& renderer, std::shared_ptr<GLShader> shader, std::shared_ptr<GLTexture2D> viewSpacePositionTexture, std::shared_ptr<GLShaderStorageBuffer> lightSSBO, std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO, std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO);

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	std::shared_ptr<GLShader> m_ComputeShader;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLShaderStorageBuffer> m_LightSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_LightIndexSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_TileIndexSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_LeftPlanesSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_RightPlanesSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_BottomPlanesSSBO;
	std::shared_ptr<GLShaderStorageBuffer> m_TopPlanesSSBO;
};