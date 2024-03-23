#include "pch.h"

#include "Application.h"

#include "Window.h"

#include "graphics/GLCommon.h"

#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace core {

	HINSTANCE Application:: s_hInstance = 0;
	const char* Application::s_WindowTitle = nullptr;
	unsigned int Application::s_Width = 0;
	unsigned int Application::s_Height = 0;
	bool Application::s_VSync = true;
	Window* Application::s_Window = nullptr;
	bool Application::s_Running = true;

	void(*Application::s_OnStart)() = 0;
	void(*Application::s_OnUpdate)() = 0;
	void(*Application::s_OnRender)() = 0;

	bool Application::Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync, void(*OnStart)(void), void(*OnUpdate)(void))
	{
		s_WindowTitle = windowTitle;
		s_Width = width;
		s_Height = height;
		s_VSync = vSync;
		s_OnStart = OnStart;
		s_OnUpdate = OnUpdate;

		Log::Init();
		CORE_TRACE("Initialized Log.");

		if (!glfwInit())
		{
			CORE_ERROR("Failed to initialize GLFW!");
			return false;
		}
		CORE_TRACE("Initialized GLFW.");

		s_Window = new Window(s_WindowTitle, s_Width, s_Height, vSync);
		if (!s_Window->m_Window) {
			CORE_ERROR("Failed to create window!");
			return false;
		}
		CORE_TRACE("Created window.");

		GLenum err = glewInit();
		if (err != GLEW_OK)
		{
			CORE_ERROR("Failed to initialize GLEW! Error: %d.", err);
			delete s_Window;
			s_Window = nullptr;
			return false;
		}
		CORE_TRACE("Initialized GLEW.");

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glfwSwapBuffers(s_Window->m_Window);

		CORE_INFO("Finished initialization.");

		return true;
	}

	void Application::Terminate()
	{
		CORE_TRACE("Terminating application...");

		if (s_Window)
			delete s_Window;

		glfwTerminate();

		Log::Terminate();
	}


	void Application::Run()
	{
		while (s_Running)
		{

		}
	}

}
