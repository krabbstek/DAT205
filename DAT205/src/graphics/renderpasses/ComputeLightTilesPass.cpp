#include "ComputeLightTilesPass.h"

#include "Globals.h"
#include "graphics/TileGrid.h"

#include <algorithm>

ComputeLightTilesPass::ComputeLightTilesPass(
	Renderer& renderer,
	std::shared_ptr<GLShader> computeShader,
	std::shared_ptr<GLTexture2D> viewSpacePositionTexture,
	std::shared_ptr<GLShaderStorageBuffer> lightSSBO,
	std::shared_ptr<GLShaderStorageBuffer> lightIndexSSBO,
	std::shared_ptr<GLShaderStorageBuffer> tileIndexSSBO)
	: RenderPass(renderer),
	m_ComputeShader(computeShader),
	m_ViewSpacePositionTexture(viewSpacePositionTexture),
	m_LightSSBO(lightSSBO),
	m_LightIndexSSBO(lightIndexSSBO),
	m_TileIndexSSBO(tileIndexSSBO)
{
	TileGrid* tileGrid = new TileGrid();

	m_LightSSBO->SetData(&g_GlobalLight, sizeof(g_GlobalLight));
	m_LightIndexSSBO->SetData(nullptr, BIT(24));

	m_LeftPlanesSSBO = std::make_shared<GLShaderStorageBuffer>(tileGrid->leftPlanes, sizeof(tileGrid->leftPlanes));
	m_RightPlanesSSBO = std::make_shared<GLShaderStorageBuffer>(tileGrid->rightPlanes, sizeof(tileGrid->rightPlanes));
	m_BottomPlanesSSBO = std::make_shared<GLShaderStorageBuffer>(tileGrid->bottomPlanes, sizeof(tileGrid->bottomPlanes));
	m_TopPlanesSSBO = std::make_shared<GLShaderStorageBuffer>(tileGrid->topPlanes, sizeof(tileGrid->topPlanes));

	delete tileGrid;

	m_ComputeShader->SetUniform1i("u_MaxNumLightsPerTile", g_MaxNumLightsPerTile);
	m_ComputeShader->SetUniform1f("u_NearPlaneDepth", g_NearPlaneDepth);
	m_ComputeShader->SetUniform1f("u_FarPlaneDepth", g_FarPlaneDepth);
	m_ComputeShader->SetUniform2f("u_ViewportSize", vec2(float(g_WindowWidth), float(g_WindowHeight)));
	m_ComputeShader->SetUniform1i("u_TileSize", g_TileSize);
}


void ComputeLightTilesPass::Render(std::vector<Renderable*>& renderables)
{
	m_ComputeShader->SetUniform1i("u_MaxNumLightsPerTile", g_MaxNumLightsPerTile);
	m_ComputeShader->SetUniform1f("u_FarPlaneDepth", g_FarPlaneDepth);
	m_ComputeShader->SetUniform2f("u_ViewportSize", float(g_WindowWidth), float(g_WindowHeight));

	m_ViewSpacePositionTexture->Bind(0);

	int zero = 0;
	m_LightIndexSSBO->SetSubData(&zero, 0, sizeof(zero));

	m_LightSSBO->Bind(3);
	m_LightIndexSSBO->Bind(4);
	m_TileIndexSSBO->Bind(5);

	m_LeftPlanesSSBO->Bind(10);
	m_RightPlanesSSBO->Bind(11);
	m_BottomPlanesSSBO->Bind(12);
	m_TopPlanesSSBO->Bind(13);

	m_ComputeShader->DispatchComputeShader(g_NumTileCols, g_NumTileRows, 1);
}