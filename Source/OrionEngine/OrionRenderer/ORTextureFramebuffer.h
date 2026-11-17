#pragma once

#include <cstdint>

#include <OrionEngine/Core/OERef.h>

#include "ORTexture.h"

namespace OrionEngine::OrionRenderer
{
    class ORTextureFramebuffer
    {
    public:
        virtual ~ORTextureFramebuffer() = default;

        virtual void Bind() const noexcept = 0;
        virtual void Unbind() const noexcept = 0;
        virtual bool IsComplete() const noexcept = 0;

        virtual uint32_t GetWidth() const noexcept = 0;
        virtual uint32_t GetHeight() const noexcept = 0;

        virtual void AttachTexture(const Ref<ORTexture>& texture) = 0;

        static Ref<ORTextureFramebuffer> CreateTextureFramebuffer() noexcept;
    };
}