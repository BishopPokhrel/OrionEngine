#pragma once
#define GLFW_INCLUDE_NONE
#include "OpenGLVertexBuffer.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLVertexArray
		{
		public:

			OpenGLVertexArray() noexcept;
			~OpenGLVertexArray() noexcept;

			void BindVertexArray() const noexcept;
			void UnbindVertexArray() const noexcept;

			void AddVertexBuffer(const OpenGLVertexBuffer& VBO) noexcept;

		private:

			GLuint m_RendererID;
		};
	}
}