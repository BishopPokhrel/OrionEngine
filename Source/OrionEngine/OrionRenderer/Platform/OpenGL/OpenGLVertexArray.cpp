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
			OE_CORE_ASSERT(VBO, "Vertex buffer is null!");

			Bind();
			VBO->Bind();

			const ORVertexBufferLayout& layout = VBO->GetLayout();
			const auto& elements = layout.GetElements();

			uint32_t index = 0;

			for (const VertexBufferElement& element : elements)
			{
				glEnableVertexAttribArray(index);

				glVertexAttribPointer(
					index,
					static_cast<GLint>(ShaderDataTypeComponentCount(element.Type)),
					GL_FLOAT,
					element.Normalized ? GL_TRUE : GL_FALSE,
					static_cast<GLsizei>(layout.GetStride()),
					reinterpret_cast<const void*>(static_cast<uintptr_t>(element.Offset))
				);

				++index;
			}

			m_VertexBuffers.push_back(VBO);
		}
	}
}