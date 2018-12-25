#include "pch.h"

#include "Window.h"

#include "graphics/GLCommon.h"

#include <GLFW/glfw3.h>

namespace core {

	Window::Window(const char* windowTitle, unsigned int width, unsigned int height, bool vSync)
		: m_WindowTitle(windowTitle), m_Width(width), m_Height(height), m_VSync(vSync)
	{
		m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), NULL, NULL);
		if (!m_Window)
		{
			CORE_ERROR("Call to glfwCreateWindow failed!");
			return;
		}

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwMakeContextCurrent(m_Window);

		if (m_VSync)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	Window::~Window()
	{
		CORE_TRACE("Call to Window destructor.");
		if (m_Window)
			glfwDestroyWindow(m_Window);
	}


	void Window::Update(float deltaTime)
	{
		glfwPollEvents();
	}

	void Window::Render()
	{
		glfwSwapBuffers(m_Window);
	}

	void Window::Clear()
	{
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
	}

}