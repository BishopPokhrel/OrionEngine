#pragma once

#include <cstdint>

#include <glm/glm.hpp>

#include <imgui.h>
#include <ImGuizmo.h>

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/OrionRenderer/ORFrameBuffer.h>
#include <OrionEngine/Core/Scene/OEScene.h>

#include "OEEditorContext.h"

namespace OrionEngine::OrionEditor
{
    struct OERay
    {
        glm::vec3 Origin;
        glm::vec3 Direction;
    };

    class OEViewport
    {
    public:
        
        explicit OEViewport(OEEditorContext* context)
            : m_EditorContext(context)
        { }
        
        bool ShowViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer, OEScene* scene);

    private:

        OERay CreateRayFromCamera(const glm::vec2& mousePos, const glm::vec2& viewportPos, const glm::vec2& viewportSize, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
        bool IntersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& boundsMin, const glm::vec3& boundsMax, float& hitDistance);

        enum class OEGizmoOperations
        {
            Translate = ImGuizmo::TRANSLATE,
            Rotation = ImGuizmo::ROTATE,
            Scale = ImGuizmo::SCALE
        };

        enum class OEGizmoSpace
        {
            World = ImGuizmo::WORLD,
            Local = ImGuizmo::LOCAL
        };

    private:
        
        OEEditorContext* m_EditorContext = nullptr;
        OEGizmoOperations m_GizmoOperations = OEGizmoOperations::Translate;
        OEGizmoSpace m_GizmoSpace = OEGizmoSpace::World;
    };
}