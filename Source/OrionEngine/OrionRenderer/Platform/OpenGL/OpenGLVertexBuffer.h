#pragma once
#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <OrionEngine/OrionRenderer/ORVertexBuffer.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLVertexBuffer : public ORVertexBuffer
		{
		public:
			OpenGLVertexBuffer(const float* vertices, size_t size) noexcept;
			~OpenGLVertexBuffer() noexcept override;

			void Bind() const noexcept override;
			void Unbind() const noexcept override;

		private:

			GLuint m_RendererID;
		};
	}
} 