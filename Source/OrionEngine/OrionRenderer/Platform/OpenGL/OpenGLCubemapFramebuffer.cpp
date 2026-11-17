#include "OpenGLCubemapFramebuffer.h"

#include <OrionEngine/Core/OELogging.h>

#include <algorithm>

namespace OrionEngine::OrionRenderer
{
	OpenGLCubemapFramebuffer::OpenGLCubemapFramebuffer()
	{
		glGenFramebuffers(1, &m_RendererID);
        m_IsComplete = false;
	}

	OpenGLCubemapFramebuffer::~OpenGLCubemapFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

    void OpenGLCubemapFramebuffer::Bind() const noexcept
    {
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_PreviousFramebuffer);
        glGetIntegerv(GL_VIEWPORT, m_PreviousViewport);

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));
    }

    void OpenGLCubemapFramebuffer::Unbind() const noexcept
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PreviousFramebuffer);

        glViewport(
            m_PreviousViewport[0],
            m_PreviousViewport[1],
            m_PreviousViewport[2],
            m_PreviousViewport[3]
        );
    }

	void OpenGLCubemapFramebuffer::AttachCubemapFace(const Ref<ORCubemap>& cubemap, uint32_t faceIndex, uint32_t mipLevel)
	{
		Ref<OpenGLCubemap> openGLCubemap = std::dynamic_pointer_cast<OpenGLCubemap>(cubemap);

		if (!openGLCubemap)
		{
			ORION_ENGINE_ERROR("Cubemap is not an OpenGL cubemap!");
			return;
		}

		AttachCubemapInternal(openGLCubemap, faceIndex, mipLevel);
	}

    void OpenGLCubemapFramebuffer::AttachCubemapInternal(const Ref<OpenGLCubemap>& cubemap, uint32_t faceIndex, uint32_t mipLevel)
    {
        m_IsComplete = false;

        if (!cubemap)
        {
            ORION_ENGINE_ERROR("Cubemap is null!");
            return;
        }

        if (faceIndex > 5)
        {
            ORION_ENGINE_ERROR("Face index is out of bounds!");
            return;
        }

        if (mipLevel >= cubemap->GetMipLevels())
        {
            ORION_ENGINE_ERROR("Cubemap mip level is out of bounds!");
            return;
        }

        GLint previousFramebuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        GLuint cubemapRendererID = cubemap->m_RendererID;

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex,
            cubemapRendererID,
            static_cast<GLint>(mipLevel)
        );

        m_IsComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        if (!m_IsComplete)
        {
            ORION_ENGINE_ERROR("Framebuffer is not complete!");
            glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
            return;
        }

        m_Width = std::max(1u, cubemap->GetWidth() >> mipLevel);
        m_Height = std::max(1u, cubemap->GetHeight() >> mipLevel);

        glBindFramebuffer(GL_FRAMEBUFFER, previousFramebuffer);
    }
}