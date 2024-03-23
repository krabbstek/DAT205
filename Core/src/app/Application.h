#pragma once

#include "Common.h"

#include <Windows.h>

namespace core {

	class CORE_API Application
	{
	public:
		static void Init(const char* windowTitle, unsigned int width, unsigned int height, bool vSync = true);
		static void Terminate();

	private:
		static HINSTANCE s_hInstance;
		static const char* s_WindowTitle;
		static unsigned int s_Width;
		static unsigned int s_Height;
		static bool s_VSync;

		friend int ::WinMain(HINSTANCE, HINSTANCE, LPSTR, int);
	};

}