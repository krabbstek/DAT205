#include "ClearColorTest.h"

#include <imgui.h>

ClearColorTest::ClearColorTest()
	: m_ClearColor(0.0f)
{
	core::Application::GetWindow()->SetClearColor(m_ClearColor);
}

ClearColorTest::~ClearColorTest()
{
	core::Application::GetWindow()->SetClearColor(core::vec3(0.0f));
}


void ClearColorTest::Render()
{
	core::Application::GetWindow()->SetClearColor(m_ClearColor);
}

void ClearColorTest::ImGuiRender()
{
	ImGui::ColorPicker3("Clear color", &m_ClearColor.r);
}