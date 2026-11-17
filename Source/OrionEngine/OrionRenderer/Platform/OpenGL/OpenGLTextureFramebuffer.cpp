#include "OpenGLTextureFramebuffer.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>

#include "OpenGLTexture.h"

namespace OrionEngine::OrionRenderer
{
	OpenGLTextureFramebuffer::OpenGLTextureFramebuffer()
	{
		glGenFramebuffers(1, &m_RendererID);

		if (m_RendererID == 0)
		{
			ORION_ENGINE_ERROR("Renderer ID is 0!");
			return;
		}
	}

	OpenGLTextureFramebuffer::~OpenGLTextureFramebuffer()
	{
		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLTextureFramebuffer::Bind() const noexcept
	{
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PreviousFramebuffer);
		glGetIntegerv(GL_VIEWPORT, &m_PreviousViewport[0]);

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Width, m_Height);
	}

	void OpenGLTextureFramebuffer::Unbind() const noexcept
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_PreviousFramebuffer);

		glViewport(
			m_PreviousViewport[0],
			m_PreviousViewport[1],
			m_PreviousViewport[2],
			m_PreviousViewport[3]
		);
	}

	void OpenGLTextureFramebuffer::AttachTexture(const Ref<ORTexture>& texture)
	{
		OE_CORE_ASSERT(texture, "Texture is null!");

		Ref<OpenGLTexture> openGLTexture = std::dynamic_pointer_cast<OpenGLTexture>(texture);

		OE_CORE_ASSERT(openGLTexture, "OpenGL texture is null!");

		AttachTextureInternal(openGLTexture);
	}

	void OpenGLTextureFramebuffer::AttachTextureInternal(const Ref<OpenGLTexture>& texture)
	{
		GLint previousFramebuffer = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

		GLuint textureRendererID = texture->m_RendererID;

		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureRendererID, 0);

		m_IsComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

		if (!m_IsComplete)
		{
			ORION_ENGINE_ERROR("Framebuffer is not complete!");
			glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
			return;
		}

		m_Width = texture->GetWidth();
		m_Height = texture->GetHeight();

		glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
	}
}