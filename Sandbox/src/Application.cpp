#include <iostream>

#include "../Core.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// If instance already running: close current instance.
	if (hPrevInstance)
		return 0;

	core::Log::Init();

	core::Log::Terminate();

	return 0;
}