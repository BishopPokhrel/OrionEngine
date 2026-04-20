#pragma once

#include <string>
#include <glm/glm.hpp>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		struct VertexShader
		{
			std::string VertexShaderSource;
		};

		struct FragmentShader
		{
			std::string FragmentShaderSource;
		};

		class ORShader
		{
		public:

			virtual void CompileAndLinkThroughShaderFiles(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) noexcept = 0;
			virtual void CompileAndLinkThroughShaderVariables(const VertexShader& vertexShader, const FragmentShader& fragmentShader) noexcept = 0;
			virtual void SetMat4(const std::string& name, const glm::mat4& value) noexcept = 0;
			virtual void SetVec3(const std::string& name, const glm::vec3& value) noexcept = 0;
			virtual void SetFloat(const std::string& name, float value) noexcept = 0;

			virtual void Bind() noexcept = 0;
			virtual void Unbind() noexcept = 0;
		};
	}
}