#pragma once

#include <cstdint>
#include <glad/glad.h>
#include <OrionEngine/OrionRenderer/ORVertexBuffer.h>

namespace OrionEngine::OrionRenderer
{
	class OpenGLVertexBuffer : public ORVertexBuffer
	{
	public:
		OpenGLVertexBuffer(const void* vertices, uint32_t size) noexcept;
		~OpenGLVertexBuffer() noexcept override;

		void Bind() const noexcept override;
		void Unbind() const noexcept override;

	private:
		GLuint m_RendererID;
	};
}