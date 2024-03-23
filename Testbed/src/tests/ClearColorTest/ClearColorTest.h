#pragma once

#include "../Test.h"

class ClearColorTest : public Test
{
public:
	ClearColorTest();
	~ClearColorTest();

	void Render();
	void ImGuiRender();

	static Test* Create() { return new ClearColorTest(); }

private:
	core::vec3 m_ClearColor;
};