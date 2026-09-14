#include "OEViewport.h"

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
    bool OEViewport::ShowViewport(
        const Ref<OrionRenderer::ORFrameBuffer>& framebuffer
    )
    {
        if (framebuffer == nullptr)
            return false;

        if (!ImGui::Begin("Viewport"))
        {
            ImGui::End();
            return false;
        }

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();

        if (viewportSize.x > 0.0f && viewportSize.y > 0.0f)
        {
            const uint32_t width =
                static_cast<uint32_t>(viewportSize.x);

            const uint32_t height =
                static_cast<uint32_t>(viewportSize.y);

            //if (width != framebuffer->GetWidth() ||
            //    height != framebuffer->GetHeight())
            //{
            //    framebuffer->Resize(width, height);
            //}

            ImGui::Image(
                static_cast<ImTextureID>(
                    static_cast<uintptr_t>(
                        framebuffer->GetColorAttachementRendererID()
                        )
                    ),
                viewportSize,
                ImVec2(0.0f, 1.0f),
                ImVec2(1.0f, 0.0f)
            );
        }

        ImGui::End();

        return true;
    }
}