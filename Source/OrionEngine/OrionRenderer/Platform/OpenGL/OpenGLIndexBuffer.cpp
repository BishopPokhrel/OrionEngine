#define GLFW_INCLUDE_NONE // just in case

#include "OpenGLIndexBuffer.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) noexcept
		{
			m_Count = count;

			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		}

		OpenGLIndexBuffer::~OpenGLIndexBuffer() noexcept
		{
			if (m_RendererID)
				glDeleteBuffers(1, &m_RendererID);
		}

		void OpenGLIndexBuffer::BindIndexBuffer() const noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		}

		void OpenGLIndexBuffer::UnbindIndexBuffer() const noexcept
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}