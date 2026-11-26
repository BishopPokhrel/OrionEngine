#pragma once

#define GLFW_INCLUDE_NONE

#include <OrionEngine/OrionRenderer/ORShadowMap.h>

#include <glad/glad.h>

namespace OrionEngine::OrionRenderer
{
    class OpenGLShadowMap : public ORShadowMap
    {
    public:

        OpenGLShadowMap() = default;
        ~OpenGLShadowMap() = default;

        bool Init(uint32_t resolution) noexcept override;
        void Release() noexcept override;

        void Bind() const noexcept override;
        void Unbind() const noexcept override;

        void BindDepthTexture(uint32_t slot) const noexcept override;

        bool Resize(uint32_t resolution) noexcept override;

        uint32_t GetDepthTextureID() const noexcept override { return m_DepthTextureID; }
        uint32_t GetResolution() const noexcept override { return m_Resolution; }

    private:

        GLuint m_RendererID = 0;

        GLuint m_Resolution = 0;
        uint32_t m_DepthTextureID = 0;

        mutable GLuint m_PreviousFramebufferID = 0;
    };
}