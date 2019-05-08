#pragma once

#include "RenderPass.h"

#include "Globals.h"
#include "graphics/opengl/OpenGL.h"
#include "meshes/FullscreenMesh.h"

enum OUTPUT_SELECTION
{
	OUTPUT_SELECTION_DEPTH,
	OUTPUT_SELECTION_NORMAL,
	OUTPUT_SELECTION_SSAO,
	OUTPUT_SELECTION_LIGHTING,
	OUTPUT_SELECTION_MOTION_BLUR,
	OUTPUT_SELECTION_BLOOM,
};

#define OUTPUT_R   1
#define OUTPUT_G   2
#define OUTPUT_B   3
#define OUTPUT_RG  4
#define OUTPUT_RB  5
#define OUTPUT_GB  6
#define OUTPUT_RGB 7

class OutputSelectionPass : public RenderPass
{
public:
	OutputSelectionPass(Renderer& renderer,
		const OUTPUT_SELECTION& outputSelection,
		std::shared_ptr<GLShader> depthShader,
		std::shared_ptr<GLShader> fullscreenShader,
		std::shared_ptr<GLShader> lightTilesOverlayShader,
		std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO,
		std::shared_ptr<GLTexture2D> viewSpacePositionTexture,
		std::shared_ptr<GLTexture2D> viewSpaceNormalTexture,
		std::shared_ptr<GLTexture2D> ssaoTexture,
		std::shared_ptr<GLTexture2D> lightingColorTexture,
		std::shared_ptr<GLTexture2D> motionBlurredTexture,
		std::shared_ptr<GLTexture2D> bloomTexture);

	virtual void Render(std::vector<Renderable*>& renderables) override;

private:
	const OUTPUT_SELECTION& m_OutputSelection;
	std::shared_ptr<GLShader> m_DepthShader;
	std::shared_ptr<GLShader> m_FullscreenShader;
	std::shared_ptr<GLShader> m_LightTilesOverlayShader;
	std::shared_ptr<GLShaderStorageBuffer> m_TileIndexSSBO;
	std::shared_ptr<GLTexture2D> m_ViewSpacePositionTexture;
	std::shared_ptr<GLTexture2D> m_ViewSpaceNormalTexture;
	std::shared_ptr<GLTexture2D> m_SSAOTexture;
	std::shared_ptr<GLTexture2D> m_LightingColorTexture;
	std::shared_ptr<GLTexture2D> m_MotionBlurredTexture;
	std::shared_ptr<GLTexture2D> m_BloomTexture;
	FullscreenMesh m_FullscreenMesh;
};