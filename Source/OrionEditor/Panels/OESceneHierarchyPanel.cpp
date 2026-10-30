#include "OESceneHierarchyPanel.h"
#include <imgui.h>

namespace OrionEngine::OrionEditor
{
    bool OESceneHierarchyPanel::ShowSceneHierachy()
    {
        if (!m_Registry || !m_EditorContext)
            return false;

        auto entities = m_Registry->GetAllEntityIDs();

        if (entities.empty())
        {
            ImGui::Text("No Game Entities yet.");
            return true;
        }

        for (ECS::OEGameEntityID id : entities)
        {
            bool isEntitySelected = (m_EditorContext->SelectedEntity == id);

            std::string name = m_Registry->GetEntityNameByID(id);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

            if (isEntitySelected)
                flags |= ImGuiTreeNodeFlags_Selected;

            bool isExpanded = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<uintptr_t>(id)), flags, "%s", name.c_str());

            if (ImGui::IsItemClicked())
                m_EditorContext->SelectedEntity = id;

            if (isExpanded)
            {
                if (m_Registry->HasComponent<ECS::OETransformComponent>(id)) ImGui::BulletText("Transform");
                if (m_Registry->HasComponent<ECS::OEPhysicsComponent>(id)) ImGui::BulletText("Physics");
                if (m_Registry->HasComponent<ECS::OERenderableComponent>(id)) ImGui::BulletText("Renderable");
                if (m_Registry->HasComponent<ECS::OECameraComponent>(id)) ImGui::BulletText("Camera");

                ImGui::TreePop();
            }
        }
        return true;
    }

    bool OESceneHierarchyPanel::DrawSceneHierarchyPanel()
    {
        ImGui::Begin("Scene Hierarchy");

        ShowSceneHierachy();

        ImGui::End();
        return true;
    }
}
