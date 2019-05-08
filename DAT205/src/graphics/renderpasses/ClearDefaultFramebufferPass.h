#pragma once

#include "RenderPass.h"

class ClearDefaultFramebufferPass : public RenderPass
{
public:
	ClearDefaultFramebufferPass(Renderer& renderer)
		: RenderPass(renderer) {}

	virtual void Render(std::vector<Renderable*>& renderables) override;
};