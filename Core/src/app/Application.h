#pragma once

#include "Common.h"

#include <Windows.h>

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

	protected:
		static HINSTANCE s_hInstance;
		static const char* s_WindowTitle;
		static unsigned int s_Width;
		static unsigned int s_Height;
		static bool s_VSync;
		static Window* s_Window;
		static bool s_Running;

		/* Function pointer to user-defined OnStart function. */
		static void(*s_OnStart)(void);
		/* Function pointer to user-defined OnUpdate function. */
		static void(*s_OnUpdate)(float);
		/* Function pointer to user-defined OnRender function. */
		static void(*s_OnRender)(void);
		/* Function pointer to user-defined OnQuit function. */
		static void(*s_OnQuit)(void);

		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	};

}