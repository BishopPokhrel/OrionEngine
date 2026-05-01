#include "OpenGLVertexArray.h"
#include "OpenGLVertex.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		OpenGLVertexArray::OpenGLVertexArray() noexcept
		{
			glGenVertexArrays(1, &m_RendererID);
		}

		OpenGLVertexArray::~OpenGLVertexArray() noexcept
		{
			if (m_RendererID != 0)
			{
				glDeleteVertexArrays(1, &m_RendererID);
				m_RendererID = 0;
			}
		}

		void OpenGLVertexArray::Bind() const noexcept
		{
			glBindVertexArray(m_RendererID);
		}

		void OpenGLVertexArray::Unbind() const noexcept
		{
			glBindVertexArray(0);
		}

		void OpenGLVertexArray::AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept
		{
			Bind();
			VBO->Bind();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
			glEnableVertexAttribArray(0);
			Unbind();
		}
	}
}