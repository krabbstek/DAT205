#pragma once

#include "graphics/RenderPass.h"

#include "Globals.h"

class ForwardLightingPass : public RenderPass
{
public:
	ForwardLightingPass(Renderer& renderer, std::shared_ptr<GLShader> shader);

	virtual void Render(std::vector<Renderable*>& renderables) override;
};