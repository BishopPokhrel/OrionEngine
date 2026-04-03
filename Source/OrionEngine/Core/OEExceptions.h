#pragma once

#include <exception>
#include <string>

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

		OENullPointerException()
			: OEBaseException("Null Pointer Exception")
		{}
	};
}