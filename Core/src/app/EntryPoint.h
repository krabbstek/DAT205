#pragma once

#include "Common.h"

#include "Application.h"

#include <Windows.h>

namespace core {

	void OnStart();
	void OnUpdate(float deltaTime);
	void OnRender();
	void OnQuit();

}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// If instance already running: close current instance.
	if (hPrevInstance)
		return 0;

	core::Application::s_hInstance = hInstance;
	core::Application::Init("DAT205", 1280, 720, true, core::OnStart, core::OnUpdate, core::OnRender, core::OnQuit);

	core::Application::Run();

	core::Application::Terminate();

	return 0;
}