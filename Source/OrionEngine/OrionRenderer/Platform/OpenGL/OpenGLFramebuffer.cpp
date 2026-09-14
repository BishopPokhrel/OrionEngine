#define GLFW_INCLUDE_NONE 

#include "OpenGLFramebuffer.h"

#include <OrionEngine/Core/OEAssert.h>

#include <glad/glad.h>

namespace OrionEngine::OrionRenderer
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		Invalidate();
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		Destroy();
	}

	void OpenGLFrameBuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
	}

	void OpenGLFrameBuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
			return;

		m_Width = width;
		m_Height = height;

		Invalidate();
	}

	void OpenGLFrameBuffer::Destroy()
	{
		if (m_ColorAttachmentRendererID != 0)
		{
			glDeleteTextures(1, &m_ColorAttachmentRendererID);
			m_ColorAttachmentRendererID = 0;
		}

		if (m_DepthAttachmentRendererID != 0)
		{
			glDeleteTextures(1, &m_DepthAttachmentRendererID);
			m_DepthAttachmentRendererID = 0;
		}

		if (m_RendererID != 0)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

    void OpenGLFrameBuffer::Invalidate()
    {
        OE_CORE_ASSERT(m_Width > 0 && m_Height > 0, "Framebuffer dimensions must be greater than 0");

        Destroy();

        glGenFramebuffers(1, &m_RendererID);
        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        // Color attachment
        glGenTextures(1, &m_ColorAttachmentRendererID);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentRendererID);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            static_cast<GLsizei>(m_Width),
            static_cast<GLsizei>(m_Height),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_ColorAttachmentRendererID,
            0
        );

        // Depth attachment
        glGenTextures(1, &m_DepthAttachmentRendererID);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachmentRendererID);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH24_STENCIL8,
            static_cast<GLsizei>(m_Width),
            static_cast<GLsizei>(m_Height),
            0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_STENCIL_ATTACHMENT,
            GL_TEXTURE_2D,
            m_DepthAttachmentRendererID,
            0
        );

        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        OE_CORE_ASSERT(
            glCheckFramebufferStatus(GL_FRAMEBUFFER) ==
            GL_FRAMEBUFFER_COMPLETE,
            "OpenGL framebuffer process is not yet complete!"
        );

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}