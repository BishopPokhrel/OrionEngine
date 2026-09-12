#include "OESceneHierarchyPanel.h"
#include <imgui.h>

namespace OrionEngine::OrionEditor
{
    bool OESceneHierarchyPanel::ShowSceneHierachy()
    {
        if (!m_Registry)
            return false;

        auto& entities = m_Registry->GetAllEntityIDs();

        if (entities.empty())
        {
            ImGui::Text("No Game Entities yet.");
            return true;
        }

        for (auto& id : entities)
        {
            bool b_Selected = (m_EditorContext->SelectedEntity == id);

            std::string name = m_Registry->GetEntityNameByID(id);

            if (ImGui::Selectable(name.c_str(), b_Selected))
            {
                m_EditorContext->SelectedEntity = id;
            }
        }

        return true;
    }

    bool OESceneHierarchyPanel::DrawSceneHierarchyPanel()
    {
        ImGui::Begin("Scene Hierachy");

        ShowSceneHierachy();

        ImGui::End();
        return true;
    }
}
