#include "OpenGLShader.h"
#include <glm/gtc/matrix_transform.hpp>

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>

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
            std::cerr << "Failed to compile shader: " << shaderName << ". Info Log: " << infoLog << "\n";
            return false;
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
                return 0;
            }

            // If failed to compile, delete shaders
            if (!Utils::CompileShader(fragmentShaderID, fragmentShaderSource.c_str(), "Fragment Shader"))
            {
                glDeleteShader(vertexShaderID);
                glDeleteShader(fragmentShaderID);
                return 0;
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

            glDeleteShader(vertexShaderID);
            glDeleteShader(fragmentShaderID);
            m_RendererID = programID;
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

        GLint OpenGLShader::GetUniformLocation(const std::string& name) const noexcept
        {
            return glGetUniformLocation(m_RendererID, name.c_str());
        }

        void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value) noexcept
        {
            OE_CORE_ASSERT(m_RendererID != 0, "Shader program not created!");
            glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]);
        }

        void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& value) noexcept
        {
            glUniform3fv(GetUniformLocation(name), 1, &value[0]);
        }

        void OpenGLShader::SetFloat(const std::string& name, float value) noexcept
        {
            glUniform1f(GetUniformLocation(name), value);
        }

        void OpenGLShader::Bind() noexcept
        {
            glUseProgram(m_RendererID);
        }

        void OpenGLShader::Unbind() noexcept
        {
            glUseProgram(0);
        }

        glm::mat4 Transform::GetModelMatrix() const
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, Position);

            model = glm::rotate(model, Rotation.x, glm::vec3(1, 0, 0));
            model = glm::rotate(model, Rotation.y, glm::vec3(0, 1, 0));
            model = glm::rotate(model, Rotation.z, glm::vec3(0, 0, 1));

            model = glm::scale(model, Scale);
            return model;
        }
	}
}