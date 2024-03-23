#include "pch.h"

#include "Application.h"

#include "Window.h"

#include "graphics/opengl/GLCommon.h"
#include "graphics/opengl/GLTexture2D.h"

#include "../vendor/stb/stb_image.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

namespace core {

	void CORE_API RenderImGui();

	HINSTANCE Application::s_hInstance = 0;
	const char* Application::s_WindowTitle = nullptr;
	unsigned int Application::s_Width = 0;
	unsigned int Application::s_Height = 0;
	bool Application::s_VSync = true;
	Window* Application::s_Window = nullptr;
	bool Application::s_Running = true;

	std::unordered_map<int, void(*)(KeyEvent&)> Application::s_KeyPressedCallbacks;
	std::unordered_map<int, void(*)(KeyEvent&)> Application::s_KeyReleasedCallbacks;

	void(*Application::s_OnStart)() = 0;
	void(*Application::s_OnUpdate)(float) = 0;
	void(*Application::s_OnRender)() = 0;
	void(*Application::s_OnQuit)() = 0;

	unsigned int _Application_Frames = 0;
	unsigned int _Application_ImGui_Frames = 0;
	unsigned int _Application_Updates = 0;
	unsigned int _Application_ImGui_Updates = 0;

	bool Application::Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync, void(*OnStart)(void), void(*OnUpdate)(float), void(*OnRender)(void), void(*OnQuit)(void))
	{
		s_WindowTitle = windowTitle;
		s_Width = width;
		s_Height = height;
		s_VSync = vSync;
		s_OnStart = OnStart;
		s_OnUpdate = OnUpdate;
		s_OnRender = OnRender;
		s_OnQuit = OnQuit;

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

		glfwSetKeyCallback(s_Window->m_Window, KeyCallback);

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

		stbi_set_flip_vertically_on_load(true);

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(s_Window->m_Window, true);
		ImGui_ImplOpenGL3_Init("#version 130");

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glfwSwapBuffers(s_Window->m_Window);

		s_OnStart();

		CORE_INFO("Finished initialization.");

		return true;
	}

	void Application::Terminate()
	{
		CORE_TRACE("Terminating application...");

		s_OnQuit();

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
		std::chrono::time_point<std::chrono::steady_clock> tPrev, tCurrent, tFpsUpsPrev, tFpsUpsCurrent;
		tPrev = std::chrono::high_resolution_clock::now();
		tFpsUpsPrev = std::chrono::high_resolution_clock::now();
		std::chrono::nanoseconds deltaTime;
		double deltaUpdate = 0.0;

		while (s_Running)
		{
			tCurrent = std::chrono::high_resolution_clock::now();
			deltaTime = tCurrent - tPrev;
			deltaUpdate += upsPerNs * deltaTime.count();
			tPrev = tCurrent;

			tFpsUpsCurrent = std::chrono::high_resolution_clock::now();
			if (tFpsUpsCurrent - tFpsUpsPrev >= std::chrono::seconds(1))
			{
				_Application_ImGui_Frames = _Application_Frames;
				_Application_ImGui_Updates = _Application_Updates;
				_Application_Frames = 0;
				_Application_Updates = 0;
				tFpsUpsPrev += std::chrono::seconds(1);

#ifdef CORE_CONTINUOUSLY_PRINT_MEMORY_STATUS
				Allocator::PrintStatus();
#endif
			}

			while (deltaUpdate >= 1.0)
			{
				Update(secondsPerUpdate);
				deltaUpdate -= 1;
			}

			Render();

			if (glfwWindowShouldClose(s_Window->m_Window))
			{
				s_Running = false;
				break;
			}

			std::this_thread::sleep_for(std::chrono::microseconds(SLEEP_FOR_MICROSECONDS));
		}
	}

	void Application::Update(float secondsPerUpdate)
	{
		s_Window->Update(secondsPerUpdate);
		s_OnUpdate(secondsPerUpdate);
		_Application_Updates++;
	}

	void Application::Render()
	{
		s_Window->Clear();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		RenderImGui();

		s_OnRender();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		s_Window->Render();
		_Application_Frames++;
	}

	void CORE_API RenderImGui()
	{
		static ImGuiWindowFlags fpsUpsFlags = ImGuiWindowFlags_NoTitleBar
			| ImGuiWindowFlags_NoScrollbar
			| ImGuiWindowFlags_NoResize
			| ImGuiWindowFlags_NoMove
			| ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoInputs
			| ImGuiWindowFlags_NoBackground;

		ImGui::Begin("FPS / UPS", false, fpsUpsFlags);
		ImGui::SetWindowPos({ 0, 0 });
		ImGui::SetWindowSize({ 128, 128 });
		ImGui::Text("FPS: %d", _Application_ImGui_Frames);
		ImGui::Text("UPS: %d", _Application_ImGui_Updates);
		ImGui::End();	
	}


	void Application::SetKeyEventCallback(void(*callback)(KeyEvent&), KeyEvent& event)
	{
		int key = event.GetKeyCode();

		switch (event.GetEventType())
		{
		case EventType::KeyPressed:
			s_KeyPressedCallbacks[key] = callback;
			break;

		case EventType::KeyReleased:
			s_KeyReleasedCallbacks[key] = callback;
			break;
		}
	}


	void Application::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		switch (action)
		{
		case GLFW_PRESS:
		case GLFW_REPEAT:
			{
				auto entry = s_KeyPressedCallbacks.find(key);
				if (entry == s_KeyPressedCallbacks.end())
					return;
				KeyPressedEvent event(key);
				s_KeyPressedCallbacks[key](event);
			}
			break;

		case GLFW_RELEASE:
			{
				auto entry = s_KeyReleasedCallbacks.find(key);
				if (entry == s_KeyReleasedCallbacks.end())
					return;
				KeyReleasedEvent event(key);
				s_KeyReleasedCallbacks[key](event);
			}
			break;
		}
	}

}
