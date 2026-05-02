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

		void OpenGLVertexArray::SetIndexBuffer(const Ref<ORIndexBuffer>& IBO) noexcept
		{
			Bind();
			IBO->Bind();
			m_IndexBuffer = IBO;
		}

		void OpenGLVertexArray::AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept
		{
			Bind();
			VBO->Bind();

			uint32_t stride = 6 * sizeof(float); // position + color

			// Position attribute (location 0)
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				(const void*)0
			);

			// Color attribute (location 1)
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,
				3,
				GL_FLOAT,
				GL_FALSE,
				stride,
				(const void*)(3 * sizeof(float))
			);

			m_VertexBuffers.push_back(VBO);
		}
	}
}