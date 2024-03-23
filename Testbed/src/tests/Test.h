#pragma once

#include <Core.h>

class Test
{
public:
	Test() {}
	virtual ~Test() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}
	virtual void ImGuiRender() {}
};