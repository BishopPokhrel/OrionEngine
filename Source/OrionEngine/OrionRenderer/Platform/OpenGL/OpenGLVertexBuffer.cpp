#define GLFW_INCLUDE_NONE
#include "OpenGLVertexBuffer.h"

#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		OpenGLVertexBuffer::OpenGLVertexBuffer(const float* vertices, uint32_t size) noexcept
		{
			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		OpenGLVertexBuffer::~OpenGLVertexBuffer() noexcept
		{
			if (m_RendererID != 0)
			{
				glDeleteBuffers(1, &m_RendererID);
				m_RendererID = 0;
			}
		}

		void OpenGLVertexBuffer::Bind() const noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}

		void OpenGLVertexBuffer::Unbind() const noexcept
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);	
		}
	}
}