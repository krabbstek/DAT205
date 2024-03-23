#pragma once

#include "Common.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace core {

	class CORE_API Log
	{
	public:
		static void Init();
		static void Terminate();

		inline static std::shared_ptr<spdlog::logger> GetConsole() { return s_Console; }

	private:
		static std::shared_ptr<spdlog::logger> s_Console;
	};

#define CORE_TRACE(...)		::core::Log::GetConsole()->trace(__VA_ARGS__)
#define CORE_INFO(...)		::core::Log::GetConsole()->info(__VA_ARGS__)
#define CORE_WARN(...)		::core::Log::GetConsole()->warn(__VA_ARGS__)
#define CORE_ERROR(...)		::core::Log::GetConsole()->error(__VA_ARGS__)

}
