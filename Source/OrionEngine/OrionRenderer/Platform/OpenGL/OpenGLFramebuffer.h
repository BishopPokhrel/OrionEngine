#pragma once

#include <OrionEngine/OrionRenderer/ORFramebuffer.h>

namespace OrionEngine::OrionRenderer
{
	class OpenGLFrameBuffer : public ORFrameBuffer
	{
	public:

		OpenGLFrameBuffer(uint32_t width, uint32_t height);
		~OpenGLFrameBuffer();

		void Bind() override;
		void Unbind() override;

		void Resize(uint32_t width, uint32_t height) override;

		uint32_t GetWidth() const noexcept override { return m_Width; }
		uint32_t GetHeight() const noexcept override { return m_Height; }

		uint32_t GetColorAttachementRendererID() const noexcept override { return m_ColorAttachmentRendererID; }
		uint32_t GetDepthAttachementRendererID() const noexcept override { return m_DepthAttachmentRendererID; }

	private:

		void Destroy();
		void Invalidate();

	private:

		uint32_t m_RendererID = 0;

		uint32_t m_ColorAttachmentRendererID = 0;
		uint32_t m_DepthAttachmentRendererID = 0;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}