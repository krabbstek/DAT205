#include "pch.h"

#include "Application.h"

#include "Window.h"

#include "graphics/GLCommon.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace core {

	void RenderImGui();

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

	bool Application::Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync, void(*OnStart)(void), void(*OnUpdate)(void), void(*OnRender)(void))
	{
		s_WindowTitle = windowTitle;
		s_Width = width;
		s_Height = height;
		s_VSync = vSync;
		s_OnStart = OnStart;
		s_OnUpdate = OnUpdate;
		s_OnRender = OnRender;

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
		GLCall(CORE_INFO("OpenGL version: {}.", glGetString(GL_VERSION)));

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(s_Window->m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glfwSwapBuffers(s_Window->m_Window);

		s_OnStart();

		CORE_INFO("Finished initialization.");

		return true;
	}

	void Application::Terminate()
	{
		CORE_TRACE("Terminating application...");

		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		if (s_Window)
			delete s_Window;

		glfwTerminate();

		Log::Terminate();
	}


	void Application::Run()
	{
		constexpr double upsPerNs = UPDATES_PER_SECOND / 1e9;
		constexpr float secondsPerUpdate = 1.0f / UPDATES_PER_SECOND;
		std::chrono::time_point<std::chrono::steady_clock> tPrev, tCurrent;
		tPrev = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds deltaTime;
		double deltaUpdate = 0.0;

		while (s_Running)
		{
			tCurrent = std::chrono::high_resolution_clock::now();
			deltaTime = tCurrent - tPrev;
			deltaUpdate += upsPerNs * deltaTime.count();
			tPrev = tCurrent;

			while (deltaUpdate >= 1.0)
			{
				Update(secondsPerUpdate);
				deltaUpdate -= 1;
			}

			Render();

			if (glfwWindowShouldClose(s_Window->m_Window))
			{
				s_Running = false;
				continue;
			}

			std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_FOR_MICROSECONDS));
		}
	}

	void Application::Update(float secondsPerUpdate)
	{
		s_Window->Update(secondsPerUpdate);
		s_OnUpdate();
	}

	void Application::Render()
	{
		s_Window->Clear();
		s_OnRender();
		//RenderImGui();
		s_Window->Render();
	}

	void RenderImGui()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Test");
		ImGui::Text("TEXT");
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

}
