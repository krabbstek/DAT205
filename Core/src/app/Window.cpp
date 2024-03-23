#include "pch.h"

#include "Window.h"

#include "graphics/opengl/GLCommon.h"

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

		glfwMakeContextCurrent(m_Window);

		glfwWindowHint(GLFW_SAMPLES, 4);

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


	void Window::SetClearColor(const vec3& clearColor) const
	{
		GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, 0.0f));
	}

	void Window::SetClearColor(const vec4& clearColor) const
	{
		GLCall(glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a));
	}

}