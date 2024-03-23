#pragma once

#include "Common.h"

#include <Windows.h>

namespace core {

	class CORE_API Window;

	class CORE_API Application
	{
	public:
		static bool Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync, void(*OnStart)(void), void(*OnUpdate)(void));
		static void Terminate();

		static void Run();

	protected:
		static HINSTANCE s_hInstance;
		static const char* s_WindowTitle;
		static unsigned int s_Width;
		static unsigned int s_Height;
		static bool s_VSync;
		static Window* s_Window;

		static void(*s_OnStart)(void);
		static void(*s_OnUpdate)(void);

		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	};

}