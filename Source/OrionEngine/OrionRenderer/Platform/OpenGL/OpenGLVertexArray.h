#pragma once
#define GLFW_INCLUDE_NONE
#include "OpenGLVertexBuffer.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/OrionRenderer/ORVertexBuffer.h>
#include <OrionEngine/OrionRenderer/ORIndexBuffer.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLVertexArray : public ORVertexArray
		{
		public:

			OpenGLVertexArray() noexcept;
			~OpenGLVertexArray() noexcept override;

			void Bind() const noexcept override;
			void Unbind() const noexcept override;

			void AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept override;
			void SetIndexBuffer(const Ref<ORIndexBuffer>& IBO) noexcept override;
			const Ref<ORIndexBuffer>& GetIndexBuffer() const noexcept override { return m_IndexBuffer; }

		private:

			GLuint m_RendererID;
			Ref<ORIndexBuffer> m_IndexBuffer;
			std::vector<Ref<ORVertexBuffer>> m_VertexBuffers;
		};
	}
}