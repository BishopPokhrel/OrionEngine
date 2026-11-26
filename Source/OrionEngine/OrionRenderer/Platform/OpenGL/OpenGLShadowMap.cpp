#include "OpenGLShadowMap.h"

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine::OrionRenderer
{
    bool OpenGLShadowMap::Init(uint32_t resolution) noexcept
    {
        if (resolution == 0)
        {
            ORION_ENGINE_ERROR("Shadow map resolution cannot be 0!");
            return false;
        }

        if (m_RendererID != 0 || m_DepthTextureID != 0)
            Release();

        GLint previousFramebuffer = 0;
        GLint previousTexture = 0;

        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &previousTexture);

        glGenFramebuffers(1, &m_RendererID);
        glGenTextures(1, &m_DepthTextureID);

        glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, resolution, resolution, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        constexpr float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTextureID, 0);

        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        const bool isComplete = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;

        glBindFramebuffer(GL_FRAMEBUFFER, static_cast<GLuint>(previousFramebuffer));
        glBindTexture(GL_TEXTURE_2D, static_cast<GLuint>(previousTexture));

        if (!isComplete)
        {
            ORION_ENGINE_ERROR("Failed to initialize shadow map framebuffer!");
            Release();
            return false;
        }

        m_Resolution = resolution;

        return true;
    }

    void OpenGLShadowMap::Release() noexcept
    {
        if (m_DepthTextureID != 0)
        {
            glDeleteTextures(1, &m_DepthTextureID);
            m_DepthTextureID = 0;
        }

        if (m_RendererID != 0)
        {
            glDeleteFramebuffers(1, &m_RendererID);
            m_RendererID = 0;
        }

        m_Resolution = 0;
        m_PreviousFramebufferID = 0;
    }

    void OpenGLShadowMap::BindDepthTexture(uint32_t slot) const noexcept
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, m_DepthTextureID);
    }

    void OpenGLShadowMap::Bind() const noexcept
    {
        GLint previousFramebuffer = 0;
        glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFramebuffer);

        m_PreviousFramebufferID = static_cast<GLuint>(previousFramebuffer);

        glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
        glViewport(0, 0, static_cast<GLsizei>(m_Resolution), static_cast<GLsizei>(m_Resolution));
    }

    void OpenGLShadowMap::Unbind() const noexcept
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_PreviousFramebufferID);
        m_PreviousFramebufferID = 0;
    }

    bool OpenGLShadowMap::Resize(uint32_t resolution) noexcept
    {
        if (resolution == 0)
        {
            ORION_ENGINE_ERROR("Shadow map resolution cannot be 0!");
            return false;
        }

        if (resolution == m_Resolution)
            return true;

        return Init(resolution);
    }
}