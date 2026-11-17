#pragma once

#define GLFW_INCLUDE_NONE

#include <OrionEngine/OrionRenderer/ORTextureFramebuffer.h>

#include <glad/glad.h>

namespace OrionEngine::OrionRenderer
{
	class OpenGLTexture;

	class OpenGLTextureFramebuffer : public ORTextureFramebuffer
	{
	public:

		OpenGLTextureFramebuffer();
		~OpenGLTextureFramebuffer();
	
		void Bind() const noexcept override;
		void Unbind() const noexcept override;
		void AttachTexture(const Ref<ORTexture>& texture) override;

		bool IsComplete() const noexcept override { return m_IsComplete; }

		uint32_t GetWidth() const noexcept override { return m_Width; }
		uint32_t GetHeight() const noexcept override { return m_Height; }

	private:

		void AttachTextureInternal(const Ref<OpenGLTexture>& texture); 

	private:

		GLuint m_RendererID = 0;

		mutable GLint m_PreviousFramebuffer = 0;
		mutable GLint m_PreviousViewport[4] = { 0, 0, 0, 0 };

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;

		bool m_IsComplete = false;
	};
}