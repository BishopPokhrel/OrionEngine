#include "OESceneManagerPanel.h"

#include <imgui.h>
#include <glm/glm.hpp>

#include <OrionEngine/Core/OETime.h>

namespace EditorUtils
{
    static void DrawMat4ReadOnly(const char* label, const glm::mat4& matrix)
    {
        ImGui::SeparatorText(label);

        if (ImGui::BeginTable(label, 4, ImGuiTableFlags_Borders))
        {
            for (int row = 0; row < 4; row++)
            {
                ImGui::TableNextRow();
                for (int col = 0; col < 4; col++)
                {
                    ImGui::TableSetColumnIndex(col);
                    ImGui::Text("%.3f", matrix[col][row]); 
                }
            }
            ImGui::EndTable();
        }
    }
}

namespace OrionEngine::OrionEditor
{
    bool OESceneManagerPanel::DrawSceneCreation()
    {
        m_OESMPDataNeeded.CreationDataNeeded->NewSceneDialog = true;
        if (m_OESMPDataNeeded.CreationDataNeeded->NewSceneDialog)
        {
            ImGui::OpenPopup("New Scene");
            m_OESMPDataNeeded.CreationDataNeeded->NewSceneDialog = false;
        }

        if (ImGui::BeginPopupModal("New Scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::InputText("Scene Name...", m_OESMPDataNeeded.CreationDataNeeded->NewSceneNameBuffer, sizeof(m_OESMPDataNeeded.CreationDataNeeded->NewSceneNameBuffer));

            if (ImGui::Button("OK"))
            {
                m_SceneName = m_OESMPDataNeeded.CreationDataNeeded->NewSceneNameBuffer;
                m_CurrentScene->SetSceneName(m_SceneName);
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

		return true;
	}

	bool OESceneManagerPanel::DrawSceneDeletion()
	{
        std::string dialogTxt = "Are you sure you want to delete Scene: " + m_SceneName + "?";
        m_OESMPDataNeeded.DeletionDataNeeded->AreYouSureDialog = true;
        if (m_OESMPDataNeeded.DeletionDataNeeded->AreYouSureDialog)
        {
            ImGui::OpenPopup(dialogTxt.c_str());
            m_OESMPDataNeeded.DeletionDataNeeded->AreYouSureDialog = false;
        }

        if (ImGui::BeginPopupModal(dialogTxt.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Yes"))
            {
                m_CurrentScene->IsCreated() ? m_CurrentScene->DeleteScene() : ImGui::Text("No scene exists, cannot delete scene %s", m_SceneName);
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();
            if (ImGui::Button("No")) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

		return true;
	}

	bool OESceneManagerPanel::DrawSceneEdit()
	{
        // We take all of things that were in DrawSceneMetadata, but make it editable
        // HOWEVER, pointers and matrices are not going to be editable. Reason: Bugs and seg faults can occurr if these things are not 
        // managed properly
        // Some things or metadata, like Counts of game entities, delta time, elsaped time, frame count, FPS, etc. are not going to be editable
        // These things are meant to be read only, not editable. 

        if (ImGui::InputText("Scene Name...", m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer, sizeof(m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer)))
        {
            m_SceneName = m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer;
            m_CurrentScene->SetSceneName(m_SceneName);
        }

        if (ImGui::CollapsingHeader("Camera"))
        {
            glm::vec3 position = m_CurrentScene->GetCurrentCamera()->GetPosition();
            glm::vec3 forward = m_CurrentScene->GetCurrentCamera()->GetForward();
            glm::vec3 right = m_CurrentScene->GetCurrentCamera()->GetRight();
            glm::vec3 up = m_CurrentScene->GetCurrentCamera()->GetUp();

            float yaw = m_CurrentScene->GetCurrentCamera()->GetYaw();
            float pitch = m_CurrentScene->GetCurrentCamera()->GetPitch();

            if (ImGui::DragFloat3("Position: ", &position[0], 0.1f)) m_CurrentScene->GetCurrentCamera()->SetPosition(position);
            ImGui::DragFloat("Yaw: ", &yaw, 0.1f);
            ImGui::DragFloat("Pitch: ", &pitch, 0.1f);
            ImGui::DragFloat3("Forward: ", &forward[0], 0.1f);
            ImGui::DragFloat3("Right: ", &right[0], 0.1f);
            ImGui::DragFloat3("Up: ", &up[0], 0.1f);
        }

		return true;
	}

    bool OESceneManagerPanel::DrawSceneMetadata()
    {
        ImGui::Text("Scene %s Metadata. Read Only.", m_SceneName.c_str());
        ImGui::Text("Scene Name: %s", m_SceneName);
        ImGui::Text("Scene Pointer: %p", m_CurrentScene);

        if (ImGui::CollapsingHeader("Scene Registry"))
        {
            ImGui::Text("Current Registry (Pointer): %p", m_CurrentScene->GetRegistry());
            ImGui::Text("Game Entity Count: %zu", m_CurrentScene->GetRegistry().GetEntityCount());
        }

        float dt = OETime::OEGetDeltaTime();
        ImGui::Text("Delta Time: %f", dt);
        ImGui::Text("Elapsed Time: %f", OETime::OEGetTime()); // OEGetTime returns s_TimeSinceStart, which is the elapsed time. See OETime.h for further details.
        ImGui::Text("Frame Count: %f", OETime::OEGetFrameCount());
        ImGui::Text("FPS: %f", dt > 0.0f ? 1.0f / dt : 0.0f);

        if (ImGui::CollapsingHeader("Camera Metadata"))
        {
            ImGui::Text("Current Camera (Pointer): %p", m_CurrentScene->GetCurrentCamera());

            EditorUtils::DrawMat4ReadOnly("Projection Matrix", m_CurrentScene->GetCurrentCamera()->GetProjection());
            EditorUtils::DrawMat4ReadOnly("View Matrix", m_CurrentScene->GetCurrentCamera()->GetView());
            EditorUtils::DrawMat4ReadOnly("View Projection Matrix", m_CurrentScene->GetCurrentCamera()->GetViewProjection());
            
            if (ImGui::CollapsingHeader("Position"))
            {
                ImGui::Text("X: %f", m_CurrentScene->GetCurrentCamera()->GetPosition().x);
                ImGui::Text("Y: %f", m_CurrentScene->GetCurrentCamera()->GetPosition().y);
                ImGui::Text("Z: %f", m_CurrentScene->GetCurrentCamera()->GetPosition().z);
            }

            ImGui::Spacing();
            ImGui::Text("Yaw: %f", m_CurrentScene->GetCurrentCamera()->GetYaw());
            ImGui::Text("Pitch: %f", m_CurrentScene->GetCurrentCamera()->GetPitch());
            
            if (ImGui::CollapsingHeader("Forward"))
            {
                ImGui::Text("X: %f", m_CurrentScene->GetCurrentCamera()->GetForward().x);
                ImGui::Text("Y: %f", m_CurrentScene->GetCurrentCamera()->GetForward().y);
                ImGui::Text("Z: %f", m_CurrentScene->GetCurrentCamera()->GetForward().z);
            }

            if (ImGui::CollapsingHeader("Right"))
            {
                ImGui::Text("X: %f", m_CurrentScene->GetCurrentCamera()->GetRight().x);
                ImGui::Text("Y: %f", m_CurrentScene->GetCurrentCamera()->GetRight().y);
                ImGui::Text("Z: %f", m_CurrentScene->GetCurrentCamera()->GetRight().z);
            }

            if (ImGui::CollapsingHeader("Up"))
            {
                ImGui::Text("X: %f", m_CurrentScene->GetCurrentCamera()->GetUp().x);
                ImGui::Text("Y: %f", m_CurrentScene->GetCurrentCamera()->GetUp().y);
                ImGui::Text("Z: %f", m_CurrentScene->GetCurrentCamera()->GetUp().z);
            }
        }

        return true;
    }

    bool OESceneManagerPanel::DrawSceneManagerPanel()
    {
        if (ImGui::Begin("Scene Manager Panel"))
        {
            if (!m_CurrentScene->IsCreated())
            {
                ImGui::Text("No scene currently created.");
                DrawSceneCreation();
            }
            else
                DrawSceneEdit();

            if (ImGui::Button("Delete Scene", ImVec2(-1, 0)))
                DrawSceneDeletion();
        }

        ImGui::End();
        return true;
    }
}