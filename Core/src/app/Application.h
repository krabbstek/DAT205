#pragma once

#include "Common.h"

#include "event/Event.h"
#include "event/KeyEvent.h"

#include <Windows.h>
#include <unordered_map>

struct GLFWwindow;

namespace core {

#define UPDATES_PER_SECOND 60
#define SLEEP_FOR_MICROSECONDS 100

	class CORE_API Window;

	class CORE_API Application
	{
	public:
		static bool Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync, void(*OnStart)(void), void(*OnUpdate)(float), void(*OnRender)(void), void(*OnQuit)(void));
		static void Terminate();

		static void Run();
		static void Update(float secondsPerUpdate);
		static void Render();

		static void SetKeyEventCallback(void(*callback)(KeyEvent&), KeyEvent& event);
		inline static void SetKeyPressedEventCallback(void(*callback)(KeyEvent&), int keycode) { s_KeyPressedCallbacks[keycode] = callback; }
		inline static void SetKeyReleasedEventCallback(void(*callback)(KeyEvent&), int keycode) { s_KeyReleasedCallbacks[keycode] = callback; }

		inline static const Window* GetWindow() { return s_Window; }

	protected:
		static HINSTANCE s_hInstance;
		static const char* s_WindowTitle;
		static unsigned int s_Width;
		static unsigned int s_Height;
		static bool s_VSync;
		static Window* s_Window;
		static bool s_Running;

		static std::unordered_map<int, void(*)(KeyEvent&)> s_KeyPressedCallbacks;
		static std::unordered_map<int, void(*)(KeyEvent&)> s_KeyReleasedCallbacks;

		/* Function pointer to user-defined OnStart function. */
		static void(*s_OnStart)(void);
		/* Function pointer to user-defined OnUpdate function. */
		static void(*s_OnUpdate)(float);
		/* Function pointer to user-defined OnRender function. */
		static void(*s_OnRender)(void);
		/* Function pointer to user-defined OnQuit function. */
		static void(*s_OnQuit)(void);

		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);

	private:
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	};

}