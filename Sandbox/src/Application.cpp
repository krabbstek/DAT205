#include <iostream>

#include "../Core.h"

int main()
{
	core::Log::Init();

	CORE_WARN("HELLO\n");

	core::Log::Terminate();

	return 0;
}