#include "pch.h"

#include "Log.h"

namespace core {

	std::shared_ptr<spdlog::logger> Log::s_Console = nullptr;


	void Log::Init()
	{
		AllocConsole();

		s_Console = spdlog::stdout_color_mt("DAT205");
		s_Console->set_pattern("%^[%T] (%l): %v%$");
		s_Console->set_level(spdlog::level::trace);
	}

	void Log::Terminate()
	{
		s_Console = nullptr;
	}

}