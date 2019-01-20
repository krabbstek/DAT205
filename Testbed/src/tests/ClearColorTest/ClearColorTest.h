#pragma once

#include "../Test.h"

class ClearColorTest : public Test
{
public:
	ClearColorTest();
	~ClearColorTest();

	void Render();
	void ImGuiRender();

private:
	core::vec3 m_ClearColor;
};