#pragma once

#include <cstdint>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionRenderer
{
    class ORShadowMap
    {
    public:

        virtual ~ORShadowMap() = default;

        virtual bool Init(uint32_t resolution) noexcept = 0;
        virtual void Release() noexcept = 0;

        virtual void Bind() const noexcept = 0;
        virtual void Unbind() const noexcept = 0;

        virtual void BindDepthTexture(uint32_t slot) const noexcept = 0;

        virtual bool Resize(uint32_t resolution) noexcept = 0;

        virtual uint32_t GetDepthTextureID() const noexcept = 0;
        virtual uint32_t GetResolution() const noexcept = 0;

        static Ref<ORShadowMap> CreateShadowMap() noexcept;
    };
}