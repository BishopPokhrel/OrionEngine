#pragma once

#include <string>
#include <iostream>

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

		inline static void Log(
			LogLevel level,
			const std::string& msg,
			const char* file,
			int line,
			const char* func)
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

			out << "[" << lvl << "] " << msg << "\n"
				<< "  FILE: " << file << "\n"
				<< "  LINE: " << line << "\n"
				<< "  FUNC: " << func << "\n\n";
		}
	};

	inline void OrionLog(
		Logging::LogLevel level,
		const std::string& msg,
		const char* file,
		int line,
		const char* func)
	{
		Logging::Log(level, msg, file, line, func);
	}
}

#define ORION_ENGINE_DEBUG(msg) \
	OrionEngine::OrionLog( \
		OrionEngine::Logging::LogLevel::Debug, \
		msg, __FILE__, __LINE__, __FUNCTION__)

#define ORION_ENGINE_INFO(msg) \
	OrionEngine::OrionLog( \
		OrionEngine::Logging::LogLevel::Info, \
		msg, __FILE__, __LINE__, __FUNCTION__)

#define ORION_ENGINE_WARNING(msg) \
	OrionEngine::OrionLog( \
		OrionEngine::Logging::LogLevel::Warning, \
		msg, __FILE__, __LINE__, __FUNCTION__)

#define ORION_ENGINE_ERROR(msg) \
	OrionEngine::OrionLog( \
		OrionEngine::Logging::LogLevel::Error, \
		msg, __FILE__, __LINE__, __FUNCTION__)

#define ORION_ENGINE_FATAL_ERROR(msg) \
	OrionEngine::OrionLog( \
		OrionEngine::Logging::LogLevel::Fatal, \
		msg, __FILE__, __LINE__, __FUNCTION__)