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

		inline static void Log(LogLevel level,
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
				(level == LogLevel::Error) ? "ERROR" : "FATAL";
			out << "[" << lvl << "] " << msg << "\n"
				<< "  FILE: " << file << "\n"
				<< "  LINE: " << line << "\n"
				<< "  FUNC: " << func << "\n\n";
		}

	};

#define ORION_ENGINE_LOG(lvl, msg) \
	do { \
		OrionEngine::Logging::Log( \
			lvl, msg, __FILE__, __LINE__, __FUNCTION__ \
		); \
	} while(0)

#define ORION_ENGINE_DEBUG(msg)		  ORION_ENGINE_LOG(Logging::LogLevel::Debug, msg)
#define	ORION_ENGINE_INFO(msg)		  ORION_ENGINE_LOG(Logging::LogLevel::Info,  msg)
#define ORION_ENGINE_WARNING(msg)	  ORION_ENGINE_LOG(Logging::LogLevel::Warning, msg)
#define ORION_ENGINE_ERROR(msg)		  ORION_ENGINE_LOG(Logging::LogLevel::Error, msg)
#define ORION_ENGINE_FATAL_ERROR(msg) ORION_ENGINE_LOG(Logging::LogLevel::Fatal, msg)
}