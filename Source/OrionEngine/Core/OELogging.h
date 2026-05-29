#pragma once

#include <iostream>
#include <sstream>
#include <string>

namespace OrionEngine
{
	class Logging
	{
	public:

		enum class LogLevel
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal
		};

		template<typename... Args>
		inline static void Log(
			LogLevel level,
			const char* file,
			int line,
			const char* func,
			Args&&... args)
		{
			std::ostream& out =
				(level == LogLevel::Error || level == LogLevel::Fatal)
				? std::cerr : std::cout;

			const char* lvl =
				(level == LogLevel::Debug) ? "DEBUG" :
				(level == LogLevel::Info) ? "INFO" :
				(level == LogLevel::Warning) ? "WARNING" :
				(level == LogLevel::Error) ? "ERROR" :
				"FATAL";

			std::stringstream ss;

			(ss << ... << args);

			out << "[" << lvl << "] "
				<< ss.str() << "\n"
				<< "  FILE: " << file << "\n"
				<< "  LINE: " << line << "\n"
				<< "  FUNC: " << func << "\n\n";
		}
	};
}

#define ORION_ENGINE_DEBUG(...) \
	OrionEngine::Logging::Log( \
		OrionEngine::Logging::LogLevel::Debug, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_INFO(...) \
	OrionEngine::Logging::Log( \
		OrionEngine::Logging::LogLevel::Info, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_WARNING(...) \
	OrionEngine::Logging::Log( \
		OrionEngine::Logging::LogLevel::Warning, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_ERROR(...) \
	OrionEngine::Logging::Log( \
		OrionEngine::Logging::LogLevel::Error, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_FATAL_ERROR(...) \
	OrionEngine::Logging::Log( \
		OrionEngine::Logging::LogLevel::Fatal, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)