#pragma once

#include "Common.h"

#include "Application.h"

#include <Windows.h>

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// If instance already running: close current instance.
	if (hPrevInstance)
		return 0;

	core::Application::s_hInstance = hInstance;
	core::Application::Init("DAT205", 1280, 720, true);



	core::Application::Terminate();

	return 0;
}