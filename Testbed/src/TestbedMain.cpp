#include <CoreMain.h>

#include "tests/Tests.h"

Test* test;

void core::OnStart()
{
	test = new ClearColorTest();
}

void core::OnUpdate(float deltaTime)
{
	test->Update(deltaTime);
}

void core::OnRender()
{
	test->Render();
	test->ImGuiRender();
}

void core::OnQuit()
{
	delete test;
}