#pragma once

#include <cstdint>

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/ORFrameBuffer.h>

namespace OrionEngine::OrionEditor
{
    class OEViewport
    {
    public:
        OEViewport() = default;

        bool ShowViewport(
            const Ref<OrionRenderer::ORFrameBuffer>& framebuffer
        );
    };
}