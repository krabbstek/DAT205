#pragma once

#include "../Renderable.h"

#include <vector>

class RenderPass
{
public:
	RenderPass(Renderer& renderer)
		: m_Renderer(renderer) {}
	virtual ~RenderPass() {}

	virtual void PrepassRender(std::vector<Renderable*>& renderables);
	virtual void Render(std::vector<Renderable*>& renderables);

protected:
	Renderer& m_Renderer;
};