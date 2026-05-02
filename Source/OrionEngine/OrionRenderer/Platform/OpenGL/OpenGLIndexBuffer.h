#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>
#include <cstdint> // for uint32_t

#include <OrionEngine/OrionRenderer/ORIndexBuffer.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLIndexBuffer : public ORIndexBuffer
		{
		public:

			OpenGLIndexBuffer(uint32_t* indices, uint32_t count) noexcept;
			~OpenGLIndexBuffer() noexcept;

			void Bind() const noexcept override;
			void Unbind() const noexcept override;

			uint32_t GetCount() const noexcept override { return m_Count; }

		private:

			GLuint m_RendererID;
			uint32_t m_Count;
		};
	}
}