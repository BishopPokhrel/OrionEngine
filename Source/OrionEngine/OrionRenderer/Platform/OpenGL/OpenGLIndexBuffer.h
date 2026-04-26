#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <cstdint> // for uint32_t

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLIndexBuffer
		{
		public:

			OpenGLIndexBuffer(uint32_t* indices, uint32_t count) noexcept;
			~OpenGLIndexBuffer() noexcept;

			void BindIndexBuffer() const noexcept;
			void UnbindIndexBuffer() const noexcept;

			uint32_t GetCount() const noexcept { return m_Count; }

		private:

			GLuint m_RendererID;
			uint32_t m_Count;
		};
	}
}