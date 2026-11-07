#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdint>

namespace OrionEngine
{
	using OELogIdentifierID = uint64_t;
	constexpr OELogIdentifierID INVALID_LOG_IDENTIFIER_ID = 0;

	class OELogging
	{
	public:

		enum class OELogLevel
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal
		};

		struct OELogEntry
		{
			OELogLevel LogLevel;
			std::string Message;
			std::string File;
			int Line;
			std::string Function;
			OELogIdentifierID LogIdentifierID;
		};

		template<typename... Args>
		inline static void Log(
			OELogLevel level,
			const char* file,
			int line,
			const char* func,
			Args&&... args)
		{
			std::ostream& out =
				(level == OELogLevel::Error || level == OELogLevel::Fatal)
				? std::cerr : std::cout;

			const char* lvl =
				(level == OELogLevel::Debug) ? "DEBUG" :
				(level == OELogLevel::Info) ? "INFO" :
				(level == OELogLevel::Warning) ? "WARNING" :
				(level == OELogLevel::Error) ? "ERROR" :
				"FATAL";

			std::stringstream ss;

			(ss << ... << args);

			out << "[" << lvl << "] "
				<< ss.str() << "\n"
				<< "  FILE: " << file << "\n"
				<< "  LINE: " << line << "\n"
				<< "  FUNC: " << func << "\n\n";

			OELogEntry entry;
			entry.LogLevel = level;
			entry.File = file;
			entry.Line = line;
			entry.Function = func;
			entry.Message = ss.str();
			entry.LogIdentifierID = m_NextLogIdentifierID++;

			if (entry.LogIdentifierID != INVALID_LOG_IDENTIFIER_ID)
				m_LogHistory.push_back(entry);
		}

		inline static std::vector<OELogEntry>& GetLogHistory() noexcept{ return m_LogHistory; }

	private:

		inline static std::vector<OELogEntry> m_LogHistory = {};
		inline static OELogIdentifierID m_NextLogIdentifierID = 1;
	};
}

#define ORION_ENGINE_DEBUG(...) \
	OrionEngine::OELogging::Log( \
		OrionEngine::OELogging::OELogLevel::Debug, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_INFO(...) \
	OrionEngine::OELogging::Log( \
		OrionEngine::OELogging::OELogLevel::Info, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_WARNING(...) \
	OrionEngine::OELogging::Log( \
		OrionEngine::OELogging::OELogLevel::Warning, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_ERROR(...) \
	OrionEngine::OELogging::Log( \
		OrionEngine::OELogging::OELogLevel::Error, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)

#define ORION_ENGINE_FATAL_ERROR(...) \
	OrionEngine::OELogging::Log( \
		OrionEngine::OELogging::OELogLevel::Fatal, \
		__FILE__, __LINE__, __FUNCTION__, \
		__VA_ARGS__)