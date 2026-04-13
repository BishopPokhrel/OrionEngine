#include "OpenGLShader.h"

#include <OrionEngine/Core/OELogging.h>

namespace Utils
{
    static bool CompileShader(GLuint shaderID, const char* source, const char* shaderName) noexcept
    {
        glShaderSource(shaderID, 1, &source, nullptr);
        glCompileShader(shaderID);

        GLint success = 0;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            char infoLog[1024];
            glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        }

        return true;
    }
}

namespace OrionEngine
{
	namespace OrionRenderer
	{

		GLuint OpenGLShader::CompileAndLinkViaGLSLShaderFiles(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) noexcept
		{
            if (vertexShaderSource.empty() || fragmentShaderSource.empty())
            {
                if (vertexShaderSource.empty())
                    ORION_ENGINE_FATAL_ERROR("Vertex Shader Source is empty!");
                return 0;
            }

            GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
            GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

            // If failed to compile, delete shaders
            if (!Utils::CompileShader(vertexShaderID, vertexShaderSource.c_str(), "Vertex Shader"))
            {
                glDeleteShader(vertexShaderID);
                glDeleteShader(fragmentShaderID);
            }

            // If failed to compile, delete shaders
            if (!Utils::CompileShader(fragmentShaderID, fragmentShaderSource.c_str(), "Fragment Shader"))
            {
                glDeleteShader(vertexShaderID);
                glDeleteShader(fragmentShaderID);
            }

            GLuint programID = glCreateProgram();
            glAttachShader(programID, vertexShaderID);
            glAttachShader(programID, fragmentShaderID);
            glLinkProgram(programID);

            GLint linkSuccess = 0;
            glGetProgramiv(programID, GL_LINK_STATUS, &linkSuccess);
            
            if (!linkSuccess)
            {
                char infoLog[1024];
                glGetProgramInfoLog(programID, sizeof(infoLog), nullptr, infoLog);
                ORION_ENGINE_ERROR("Failed to link shader");
                glDeleteShader(vertexShaderID);
                glDeleteShader(fragmentShaderID);
                return 0;
            }

            return programID;
		}

		GLuint OpenGLShader::CompileAndLinkViaGLSLShaderVariables(const VertexShader& vertexShader, const FragmentShader& fragmentShader) noexcept
		{
            if (vertexShader.VertexShaderSource.empty() || fragmentShader.FragmentShaderSource.empty())
            {
                ORION_ENGINE_FATAL_ERROR("Shader source is empty!");
                return 0;
            }

            return CompileAndLinkViaGLSLShaderFiles(vertexShader.VertexShaderSource, fragmentShader.FragmentShaderSource);
		}
	}
}