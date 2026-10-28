#include "OEViewport.h"

#include <imgui.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/OETime.h>
#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine::OrionEditor
{
    bool OEViewport::ShowViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer, OEScene* scene)
    {
        if (framebuffer == nullptr || !scene || !m_EditorContext)
            return false;

        if (!ImGui::Begin("Viewport"))
        {
            ImGui::End();
            return false;
        }

        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        bool imageSubmitted = false;

        if (viewportSize.x > 0.0f && viewportSize.y > 0.0f)
        {
            ImGui::Image(static_cast<ImTextureID>(static_cast<uintptr_t>(framebuffer->GetColorAttachementRendererID())), viewportSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));
            imageSubmitted = true;
        }


        if (imageSubmitted)
        {
            const ImVec2 viewportImagePos = ImGui::GetItemRectMin();
            const ImVec2 viewportImageSize = ImGui::GetItemRectSize();
            const bool isItemHovered = ImGui::IsItemHovered();

            // Entity selection
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isItemHovered && !ImGuizmo::IsOver() && !ImGuizmo::IsUsing())
            {
                const ImVec2 mousPosition = ImGui::GetMousePos();

                const glm::vec2 mousePos(mousPosition.x, mousPosition.y);
                const glm::vec2 imagePos(viewportImagePos.x, viewportImagePos.y);
                const glm::vec2 imageSize(viewportImageSize.x, viewportImageSize.y);

                const OrionRenderer::ORCamera& camera = scene->GetCurrentCamera();
                const OERay ray = CreateRayFromCamera(mousePos, imagePos, imageSize, camera.GetView(), camera.GetProjection());

                // Access the scene's registry
                ECS::OEECSRegistry& registry = scene->GetRegistry();
                const std::vector<ECS::OEGameEntityID> entityIDs = registry.GetAllEntityIDs();

                ECS::OEGameEntityID closetEntity = ECS::INVALID_GAME_ENTITY_ID;
                float closetDistance = std::numeric_limits<float>::infinity();

                for (const auto& entityID : entityIDs)
                {
                    if (!registry.HasComponent<ECS::OETransformComponent>(entityID))
                        continue;

                    if (!registry.HasComponent<ECS::OERenderableComponent>(entityID))
                        continue;

                    auto* transform = registry.GetComponent<ECS::OETransformComponent>(entityID);
                    auto* render = registry.GetComponent<ECS::OERenderableComponent>(entityID);

                    if (!transform || !render)
                        continue;

                    if (!render->b_Visible == true || !render->Mesh)
                        continue;

                    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform->Position)
                        * glm::mat4_cast(glm::quat(transform->Rotation))
                        * glm::scale(glm::mat4(1.0f), transform->Scale);

                    glm::mat4 inverseModelMatrix = glm::inverse(modelMatrix);

                    glm::vec4 localOrigin4 = inverseModelMatrix * glm::vec4(ray.Origin, 1);
                    glm::vec4 localDirection4 = inverseModelMatrix * glm::vec4(ray.Direction, 0);
                    glm::vec3 localRayOrigin = glm::vec3(localOrigin4);
                    glm::vec3 localRayDirection = glm::normalize(glm::vec3(localDirection4));

                    const glm::vec3 boundsMin = glm::vec3(-0.5f, -0.5f, -0.5f);
                    const glm::vec3 boundsMax = glm::vec3(0.5f, 0.5f, 0.5f);
                    float hitDistance = 0.0f;

                    if (IntersectRayAABB(localRayOrigin, localRayDirection, boundsMin, boundsMax, hitDistance))
                    {
                        glm::vec3 localHitPosition = localRayOrigin + hitDistance * localRayDirection;
                        glm::vec4 worldHitPosition = modelMatrix * glm::vec4(localHitPosition, 1);
                        float worldDistance = glm::length(glm::vec3(worldHitPosition) - ray.Origin);

                        if (worldDistance < closetDistance)
                        {
                            closetDistance = worldDistance;
                            closetEntity = entityID;
                        }
                    }
                }

                m_EditorContext->SelectedEntity = closetEntity;
            }

            // Camera movement
            OrionRenderer::ORCamera& camera = scene->GetCurrentCamera();

            static bool isRotatingCamera = false;
            static ImVec2 lastMousePosition(0.0f, 0.0f);

            if (isItemHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                lastMousePosition = ImGui::GetMousePos();
                isRotatingCamera = true;
            }

            if (isRotatingCamera && ImGui::IsMouseDown(ImGuiMouseButton_Right))
            {
                glm::vec3 movement(0.0f);

                if (ImGui::IsKeyDown(ImGuiKey_W) || ImGui::IsKeyDown(ImGuiKey_UpArrow)) movement += camera.GetForward();
                if (ImGui::IsKeyDown(ImGuiKey_S) || ImGui::IsKeyDown(ImGuiKey_DownArrow)) movement -= camera.GetForward();
                if (ImGui::IsKeyDown(ImGuiKey_A) || ImGui::IsKeyDown(ImGuiKey_LeftArrow)) movement -= camera.GetRight();
                if (ImGui::IsKeyDown(ImGuiKey_D) || ImGui::IsKeyDown(ImGuiKey_RightArrow)) movement += camera.GetRight();

                if (glm::length(movement) > 0.0f)
                    camera.Translate(glm::normalize(movement) * 5.0f * OETime::OEGetDeltaTime());

                ImVec2 mousePosition = ImGui::GetMousePos();

                float deltaX = mousePosition.x - lastMousePosition.x;
                float deltaY = mousePosition.y - lastMousePosition.y;

                constexpr float sensitivity = 0.003f;

                camera.Rotate(deltaX * sensitivity, -deltaY * sensitivity);

                lastMousePosition = mousePosition;
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                isRotatingCamera = false;

            // Gizmos
            if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
            {
                // Gizmo Operation Mode

                if (ImGui::IsKeyPressed(ImGuiKey_W)) m_GizmoOperations = OEGizmoOperations::Translate;
                if (ImGui::IsKeyPressed(ImGuiKey_E)) m_GizmoOperations = OEGizmoOperations::Rotation;
                if (ImGui::IsKeyPressed(ImGuiKey_R)) m_GizmoOperations = OEGizmoOperations::Scale;
                if (ImGui::IsKeyPressed(ImGuiKey_Q)) m_GizmoOperations = OEGizmoOperations::None;

                // Camera 
                const OrionRenderer::ORCamera& camera = scene->GetCurrentCamera();
                const glm::mat4& projectionMatrix = camera.GetProjection();
                const glm::mat4& viewMatrix = camera.GetView();

                // ECS (Transform and Renderable)
                ECS::OEECSRegistry& registry = scene->GetRegistry();
                ECS::OETransformComponent* transform = registry.HasComponent<ECS::OETransformComponent>(m_EditorContext->SelectedEntity) ? registry.GetComponent<ECS::OETransformComponent>(m_EditorContext->SelectedEntity) : nullptr;
                ECS::OERenderableComponent* render = registry.HasComponent<ECS::OERenderableComponent>(m_EditorContext->SelectedEntity) ? registry.GetComponent<ECS::OERenderableComponent>(m_EditorContext->SelectedEntity) : nullptr;

                if (transform && render && render->b_Visible)
                {
                    // Construct the Model matrix
                    glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), transform->Position) * glm::mat4_cast(glm::quat(transform->Rotation)) * glm::scale(glm::mat4(1.0f), transform->Scale);

                    ImGuizmo::SetOrthographic(true);
                    ImGuizmo::SetDrawlist();
                    ImGuizmo::SetRect(viewportImagePos.x, viewportImagePos.y, viewportImageSize.x, viewportImageSize.y);

                    if (m_GizmoOperations != OEGizmoOperations::None)
                    {
                        if (ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix), static_cast<ImGuizmo::OPERATION>(m_GizmoOperations), ImGuizmo::LOCAL, glm::value_ptr(modelMatrix)))
                        {
                            glm::vec3 position;
                            glm::quat rotation;
                            glm::vec3 scale;
                            glm::vec3 skew;
                            glm::vec4 perspective;

                            if (glm::decompose(modelMatrix, scale, rotation, position, skew, perspective))
                            {
                                transform->Position = position;
                                transform->Rotation = glm::eulerAngles(rotation);
                                transform->Scale = scale;
                            }
                        }
                    }
                }
            }
        }

        ImGui::End();

        return true;
    }

    OERay OEViewport::CreateRayFromCamera(const glm::vec2& mousePos, const glm::vec2& viewportPos, const glm::vec2& viewportSize, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
    {
        const float xNDC = 2.0f * (mousePos.x - viewportPos.x) / viewportSize.x - 1.0f;
        const float yNDC = 1.0f - 2.0f * (mousePos.y - viewportPos.y) / viewportSize.y;

        const glm::mat4 inverseViewProjection = glm::inverse(projectionMatrix * viewMatrix);

        glm::vec4 nearPoint = inverseViewProjection * glm::vec4(xNDC, yNDC, -1.0f, 1.0f);
        glm::vec4 farPoint = inverseViewProjection * glm::vec4(xNDC, yNDC, 1.0f, 1.0f);

        nearPoint /= nearPoint.w;
        farPoint /= farPoint.w;

        OERay ray;
        ray.Origin = glm::vec3(nearPoint);
        ray.Direction = glm::normalize(glm::vec3(farPoint - nearPoint));

        return ray;
    }

    bool OEViewport::IntersectRayAABB(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const glm::vec3& boundsMin, const glm::vec3& boundsMax, float& hitDistance)
    {
        float tMin = 0.0f;
        float tMax = std::numeric_limits<float>::infinity();

        for (int axis = 0; axis < 3; ++axis)
        {
            if (std::abs(rayDirection[axis]) < 1e-6f)
            {
                if (rayOrigin[axis] < boundsMin[axis] || rayOrigin[axis] > boundsMax[axis])
                    return false; 

                continue;
            }

            float t1 = (boundsMin[axis] - rayOrigin[axis]) / rayDirection[axis];
            float t2 = (boundsMax[axis] - rayOrigin[axis]) / rayDirection[axis];

            if (t1 > t2)
                std::swap(t1, t2);

            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax)
                return false; 

        }

        hitDistance = tMin;
        return true;
    }
}