#include "pch.h"

#include "Log.h"

namespace core {

	std::shared_ptr<spdlog::logger> Log::s_Console = nullptr;


	void Log::Init()
	{
		s_Console = spdlog::stdout_color_mt("DAT205");
		s_Console->set_level(spdlog::level::trace);
	}

	void Log::Terminate()
	{
		s_Console = nullptr;
	}

}