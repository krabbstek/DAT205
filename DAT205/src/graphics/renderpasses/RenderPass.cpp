#include "RenderPass.h"

void RenderPass::PrepassRender(std::vector<Renderable*>& renderables)
{
	for (Renderable* renderable : renderables)
		renderable->PrepassRender(m_Renderer);
}

void RenderPass::Render(std::vector<Renderable*>& renderables)
{
	for (Renderable* renderable : renderables)
		renderable->Render(m_Renderer);
}