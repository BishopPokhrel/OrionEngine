#pragma once

#include <glm/glm.hpp>
#include <cstdint>

#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
    class OEViewport
    {
    public:
        OEViewport() = default;

        void SetScene(ECS::OEECSRegistry* scene) { m_Scene = scene; }
        void SetCamera(OrionRenderer::ORCamera* camera) { m_Camera = camera; }

        // Call every frame (renders scene into viewport)
        bool OnUpdate(float dt)
        {
            if (!m_Scene || !m_Camera)
                return false;

            // In a real version:
            // 1. bind framebuffer
            // 2. clear
            // 3. render scene using camera

            return true;
        }

        // Draw ImGui viewport window
        bool ShowViewport()
        {
            ImGui::Begin("Viewport");

            // In real engine:
            // ImGui::Image(m_FramebufferTexture, m_Size);

            ImGui::Text("Viewport Rendering Area");

            ImGui::End();
            return true;
        }

    private:
        ECS::OEECSRegistry* m_Scene = nullptr;
        OrionRenderer::ORCamera* m_Camera = nullptr;

        glm::vec2 m_Size = { 1280, 720 };
    };
}