#pragma once

#include <exception>
#include <string>
#include <sstream>

namespace Utils
{
	inline void AppendToStream(std::ostringstream&) {}
	template<typename First, typename... Rest>
	void AppendToStream(std::ostringstream& ss, const First& first, const Rest&... rest) 
	{
		ss << first;
		AppendToStream(ss, rest...);
	}

	template<typename... Args>
	std::string BuildMessage(const Args&... args) 
	{
		std::ostringstream ss;
		AppendToStream(ss, args...);
		return ss.str();
	}
}

namespace OrionEngine
{
	class OEBaseException : public std::exception
	{
	public:

		OEBaseException() = default;
		const char* what() const noexcept override { return m_ExceptionMsg.c_str(); }

	protected:

		template<typename... Args>
		explicit OEBaseException(const std::string& exceptionMsg, const Args&... otherText) noexcept // otherText parameter is meant to be completely optional.
			: m_ExceptionMsg(exceptionMsg)
		{}

	private:

		std::string m_ExceptionMsg;
	};

	class OENullPointerException : public OEBaseException
	{
	public:

		template<typename Pointer, typename... Args>
		explicit OENullPointerException(Pointer* ptr, const std::string& name, const Args&... additional)
			: OEBaseException(
				Utils::BuildMessage("Null pointer: ", name, " was nullptr. ", "Pointer: ", ptr, additional...)
			)
		{}
	};
}