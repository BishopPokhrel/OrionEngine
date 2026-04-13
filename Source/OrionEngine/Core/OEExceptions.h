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
		{
		}

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
		{
		}
	};

	class OEResourceNotFoundException : public OEBaseException
	{
	public:

		template<typename ResourceType, typename... Args>
		explicit OEResourceNotFoundException(ResourceType type, const std::string& resourceName, const Args&... additional)
			: OEBaseException(
				Utils::BuildMessage("Resource of type ", type, " with name ", resourceName, " could not be found ", additional...)
			)
		{
		}
	};

	class OEInvalidFileFormatUsedException : public OEBaseException
	{
	public:

		template<typename... Args>
		explicit OEInvalidFileFormatUsedException(
			const std::string& expectedFormat, const std::string& actualFormat, const std::string& fileName, const Args&... additional)
			: OEBaseException(
				Utils::BuildMessage(
					"Invalid file format!\n",
					"File: ", fileName, "\n",
					"Expected: ", expectedFormat, "\n",
					"Actual: ", actualFormat, "\n",
					additional...
				)
			)
		{
		}
	};

	class OEInvalidNumberOfSceneException : public OEBaseException
	{
	public:

		template<typename... Args>
		explicit OEInvalidNumberOfSceneException(const std::string& name, size_t currentNumOfScenes, const Args&... additional)
			: OEBaseException(Utils::BuildMessage("Only 1 Scene is allowed, got ", currentNumOfScenes, ".", additional...))
		{}
	};

	class OEShaderCompilationFailedException : public OEBaseException
	{
	public:
		template<typename... Args>
		explicit OEShaderCompilationFailedException(const std::string& shaderName, char infoLog[1024], const Args&... additional)
			: OEBaseException(Utils::BuildMessage("[ORION RENDERER] Failed to compile shader ", shaderName, ".\n", infoLog, additional...))
		{}
	};

	class OEShaderIsEmptyException : public OEBaseException
	{
	public:

		template<typename... Args>
		explicit OEShaderIsEmptyException(const Args&... additional)
			: OEBaseException(Utils::BuildMessage("[ORION RENDERER] Shader soure is empty!", additional...))
		{}
	};

	class OEFailedToLinkShaderException : public OEBaseException
	{
	public:

		template<typename... Args>
		explicit OEFailedToLinkShaderException(char infoLog[1024], const Args&... additional)
			: OEBaseException(Utils::BuildMessage("[ORION RENDERER] Shader linking program failed!\n", infoLog, additional...))
		{}
	};
}