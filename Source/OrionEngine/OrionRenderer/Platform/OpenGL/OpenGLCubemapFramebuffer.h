#pragma once

#define GLFW_INCLUDE_NONE

#include <OrionEngine/OrionRenderer/ORCubemapFramebuffer.h>

#include <glad/glad.h>

#include "OpenGLCubemap.h"

namespace OrionEngine::OrionRenderer
{
	class OpenGLCubemapFramebuffer : public ORCubemapFramebuffer
	{
	public:

		OpenGLCubemapFramebuffer();
		~OpenGLCubemapFramebuffer() override;

		void Bind() const noexcept override;
		void Unbind() const noexcept override;

		bool IsComplete() const noexcept override { return m_IsComplete; }

		void AttachCubemapFace(const Ref<ORCubemap>& cubemap, uint32_t faceIndex, uint32_t mipLevel = 0) override;

		uint32_t GetWidth() const noexcept override { return m_Width; }
		uint32_t GetHeight() const noexcept override { return m_Height; }

	private:

		void AttachCubemapInternal(const Ref<OpenGLCubemap>& cubemap, uint32_t faceIndex, uint32_t mipLevel);

	private:

		GLuint m_RendererID = 0;

		mutable GLint m_PreviousFramebuffer = 0;
		mutable GLint m_PreviousViewport[4] = { 0, 0, 0, 0 };

		bool m_IsComplete = false;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}