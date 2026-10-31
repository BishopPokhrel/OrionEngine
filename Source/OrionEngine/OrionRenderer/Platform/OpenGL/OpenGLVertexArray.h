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
		class OpenGLVertexArray
		{
		public:

			OpenGLVertexArray() noexcept;
			~OpenGLVertexArray() noexcept;

			void Bind() const noexcept;
			void Unbind() const noexcept;

			void AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept;
			void SetIndexBuffer(const Ref<ORIndexBuffer>& IBO) noexcept;
			const Ref<ORIndexBuffer>& GetIndexBuffer() const noexcept { return m_IndexBuffer; }

		private:

			GLuint m_RendererID;
			Ref<ORIndexBuffer> m_IndexBuffer;
			std::vector<Ref<ORVertexBuffer>> m_VertexBuffers;
		};
	}
}