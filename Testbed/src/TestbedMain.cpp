#include <CoreMain.h>
#include <imgui.h>

#include "tests/Tests.h"

Test* test;
unsigned int currentTestIndex;

void StartTest()
{
	CORE_INFO("Started test \"{}\"", testEntries[currentTestIndex].name);
	test = testEntries[currentTestIndex].CreateFcn();
}

void QuitTest()
{
	if (test)
	{
		CORE_INFO("Quit test");
		delete test;
		test = nullptr;
		core::Application::GetWindow()->SetClearColor(core::vec3(0.0f, 0.0f, 0.0f));
	}
	currentTestIndex = -1;

	core::Allocator::PrintStatus();
}


unsigned int ListTestButtons()
{
	unsigned int selectedIndex = -1;
	for (int i = 0; i < numTestEntries; i++)
	{
		if (ImGui::Button(testEntries[i].name))
			selectedIndex = i;
	}
	return selectedIndex;
}


void core::OnStart()
{
	while (testEntries[numTestEntries].CreateFcn)
		numTestEntries++;

	QuitTest();
}

void core::OnUpdate(float deltaTime)
{
	if (test)
		test->Update(deltaTime);
}

void core::OnRender()
{
	ImGui::Begin("Tests");
	if (test)
	{
		if (ImGui::Button("<-"))
		{
			QuitTest();
			ListTestButtons();
		}
		else
		{
			test->Render();
			test->ImGuiRender();
		}
	}
	else
	{
		currentTestIndex = ListTestButtons();
		if (currentTestIndex != -1)
		{
			StartTest();
			if (test)
			{
				test->Render();
				test->ImGuiRender();
			}
		}
	}
	ImGui::End();
}

void core::OnQuit()
{
	QuitTest();
}