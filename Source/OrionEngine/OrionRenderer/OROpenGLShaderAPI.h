#pragma once

#include "ORShader.h"
#include "Platform/OpenGL/OpenGLShader.h"

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OROpenGLShaderAPI : public ORShader
		{
		public:

			OrionEngine::Scope<OpenGLShader> shaderAPI;

			OROpenGLShaderAPI()
			{
				shaderAPI = OrionEngine::CreateScope<OpenGLShader>();
			}

			void CompileAndLinkThroughShaderFiles(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) noexcept override
			{
				shaderAPI->CompileAndLinkViaGLSLShaderFiles(vertexShaderSource, fragmentShaderSource);
			}

			void CompileAndLinkThroughShaderVariables(const VertexShader& vertexShader, const FragmentShader& fragmentShader) noexcept override
			{
				shaderAPI->CompileAndLinkViaGLSLShaderVariables(vertexShader, fragmentShader);
			}

			void SetMat4(const std::string& name, const glm::mat4& value) noexcept override
			{
				shaderAPI->SetMat4(name, value);
			}

			void SetVec3(const std::string& name, const glm::vec3& value) noexcept override
			{
				shaderAPI->SetVec3(name, value);
			}

			void SetFloat(const std::string& name, float value) noexcept override
			{
				shaderAPI->SetFloat(name, value);
			}

			void Bind() noexcept override
			{
				shaderAPI->Bind();
			}

			void Unbind() noexcept override
			{
				shaderAPI->Unbind();
			}
		};
	}
}