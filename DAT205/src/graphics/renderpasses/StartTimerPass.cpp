#include "StartTimerPass.h"

StartTimerPass::StartTimerPass(Renderer& renderer, std::shared_ptr<Timer> timer)
	: RenderPass(renderer), m_Timer(timer)
{
}

StartTimerPass::StartTimerPass(Renderer& renderer, std::shared_ptr<CPUTimer> timer)
	: RenderPass(renderer), m_Timer(timer)
{
}

StartTimerPass::StartTimerPass(Renderer& renderer, std::shared_ptr<GLTimer> timer)
	: RenderPass(renderer), m_Timer(timer)
{
}

void StartTimerPass::Render(std::vector<Renderable*>&)
{
	m_Timer->Start();
}