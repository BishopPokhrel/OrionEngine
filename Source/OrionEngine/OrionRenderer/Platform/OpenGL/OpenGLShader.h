#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include <OrionEngine/OrionRenderer/ORShader.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		struct Transform
		{
			glm::vec3 Position;
			glm::vec3 Rotation;
			glm::vec3 Scale;

			glm::mat4 GetModelMatrix() const;
		};

		class OpenGLShader
		{
		public:

			GLuint CompileAndLinkViaGLSLShaderFiles(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) noexcept;
			GLuint CompileAndLinkViaGLSLShaderVariables(const VertexShader& vertexShader, const FragmentShader& fragmentShaderSource) noexcept;
			GLint GetUniformLocation(const std::string& name) const noexcept;

			void SetMat4(const std::string& name, const glm::mat4& value) noexcept;
			void SetVec3(const std::string& name, const glm::vec3& value) noexcept;
			void SetFloat(const std::string& name, float value) noexcept;

			void Bind() noexcept;
			void Unbind() noexcept;

		private:

			GLuint m_RendererID = 0;
		};
	}
}