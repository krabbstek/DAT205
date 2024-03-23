#include "pch.h"

#include "Application.h"

#include <Windows.h>
#include <GLFW/glfw3.h>

namespace core {

	HINSTANCE Application:: s_hInstance = 0;
	const char* Application::s_WindowTitle = nullptr;
	unsigned int Application::s_Width = 0;
	unsigned int Application::s_Height = 0;
	bool Application::s_VSync = true;

	void Application::Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync)
	{
		s_WindowTitle = windowTitle;
		s_Width = width;
		s_Height = height;
		s_VSync = vSync;

		Log::Init();

		CORE_INFO("Initializing application...");

		CORE_INFO("Finished initialization.");
	}

	void Application::Terminate()
	{
		CORE_INFO("Terminating application.");

		Log::Terminate();
	}

}
