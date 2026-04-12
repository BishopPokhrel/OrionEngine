#pragma once
#define GLFW_INCLUDE_NONE

#include <glad/glad.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLVertexBuffer
		{
		public:
			OpenGLVertexBuffer(const float* vertices, size_t size) noexcept;
			~OpenGLVertexBuffer() noexcept;

			void BindVertexBuffer() const noexcept;
			void UnbindVertexBuffer() const noexcept;

		private:

			GLuint m_RendererID;
		};
	}
} 