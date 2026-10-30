#include "OESceneManagerPanel.h"

#include <imgui.h>
#include <glm/glm.hpp>

#include <OrionEngine/Core/OETime.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <OrionEditor/UI/OEEditorTheme.h>

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
    bool OESceneManagerPanel::DrawSceneDeletion()
    {
        std::string dialogTxt = "Are you sure you want to delete Scene: " + m_SceneName + "?";
        if (ImGui::Button("Delete Scene?"))
        {
            ImGui::OpenPopup(dialogTxt.c_str());
        }

        if (ImGui::BeginPopupModal(dialogTxt.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::Button("Yes"))
            {
                m_CurrentScene->DeleteScene();
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

        ImGui::Text("Scene Name...");
        ImGui::SameLine(100.0f);
        if (ImGui::InputText("##SceneName", m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer, sizeof(m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer)))
        {
            m_SceneName = m_OESMPDataNeeded.EditableDataNeeded->EditableSceneNameBuffer;
            m_CurrentScene->SetSceneName(m_SceneName);
        }

        if (ImGui::CollapsingHeader("Camera"))
        {
            glm::vec3 position = m_CurrentScene->GetCurrentCamera().GetPosition();
            glm::vec3 forward = m_CurrentScene->GetCurrentCamera().GetForward();
            glm::vec3 right = m_CurrentScene->GetCurrentCamera().GetRight();
            glm::vec3 up = m_CurrentScene->GetCurrentCamera().GetUp();

            float yaw = m_CurrentScene->GetCurrentCamera().GetYaw();
            float pitch = m_CurrentScene->GetCurrentCamera().GetPitch();

            if (ImGui::BeginTable("CameraEditPositionTable", 4, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
                ImGui::TableSetupColumn("X");
                ImGui::TableSetupColumn("Y");
                ImGui::TableSetupColumn("Z");

                ImGui::TableNextRow();
                
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Position");

                ImGui::TableSetColumnIndex(1);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##CameraEditPositionX", &position.x, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##CameraEditPositionY", &position.y, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##CameraPositionZ", &position.z, 0.1f, 0.0f, 0.0f, "%.2f");

                m_CurrentScene->GetCurrentCamera().SetPosition(position);

                ImGui::EndTable();
            }
            ImGui::Text("Yaw");
            ImGui::SameLine(100.0f);
            ImGui::DragFloat("##Yaw", &yaw, 0.1f, 0.0f, 0.0f, "%.2f");

            ImGui::Text("Pitch");
            ImGui::SameLine(100.0f);
            ImGui::DragFloat("##Pitch", &pitch, 0.1f, 0.0f, 0.0f, "%.2f");

            if (ImGui::BeginTable("ForwardTable", 4, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
                ImGui::TableSetupColumn("X");
                ImGui::TableSetupColumn("Y");
                ImGui::TableSetupColumn("Z");

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Forward");

                ImGui::TableSetColumnIndex(1);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##ForwardX", &forward.x, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##ForwardY", &forward.y, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##ForwardZ", &forward.z, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::EndTable();
            }

            if (ImGui::BeginTable("RightTable", 4, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
                ImGui::TableSetupColumn("X");
                ImGui::TableSetupColumn("Y");
                ImGui::TableSetupColumn("Z");

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Right");

                ImGui::TableSetColumnIndex(1);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##RightX", &right.x, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##RightY", &right.y, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##RightZ", &right.z, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::EndTable();
            }

            if (ImGui::BeginTable("UpTable", 4, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
                ImGui::TableSetupColumn("X");
                ImGui::TableSetupColumn("Y");
                ImGui::TableSetupColumn("Z");

                ImGui::TableNextRow();

                ImGui::TableSetColumnIndex(0);
                ImGui::Text("Up");

                ImGui::TableSetColumnIndex(1);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Button("X", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##UpX", &up.x, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(2);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##UpY", &up.y, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::TableSetColumnIndex(3);
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                ImGui::PopStyleColor(2);
                ImGui::SameLine();
                ImGui::SetNextItemWidth(-1.0f);
                ImGui::DragFloat("##UpZ", &up.z, 0.1f, 0.0f, 0.0f, "%.2f");

                ImGui::EndTable();
            }
        }

        return true;
    }

    bool OESceneManagerPanel::DrawSceneMetadata()
    {
        m_SceneName = m_CurrentScene->GetSceneName(); 

        ImGui::Text("Scene Metadata. Read Only.");

        ImGui::Text("Scene Name:");
        ImGui::SameLine();
        ImGui::PushFont(OEEditorTheme::SemiBoldFont);
        ImGui::Text("%s", m_SceneName.c_str());
        ImGui::PopFont();

        if (ImGui::CollapsingHeader("Scene Registry"))
        {
            ImGui::PushFont(OEEditorTheme::SemiBoldFont);
            ImGui::Text("Current Registry (Pointer):");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%p", m_CurrentScene->GetRegistry());

            ImGui::PushFont(OEEditorTheme::SemiBoldFont);
            ImGui::Text("Game Entity Count:");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%zu", m_CurrentScene->GetRegistry().GetEntityCount());
        }

        float dt = OETime::OEGetDeltaTime();

        ImGui::PushFont(OEEditorTheme::SemiBoldFont);
        ImGui::Text("Delta Time:");
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::Text("%f", dt);

        ImGui::PushFont(OEEditorTheme::SemiBoldFont);
        ImGui::Text("Elapsed Time:"); // OEGetTime returns s_TimeSinceStart, which is the elapsed time. See OETime.h for further details.
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::Text("%f", OETime::OEGetTime());

        ImGui::PushFont(OEEditorTheme::SemiBoldFont);
        ImGui::Text("Frame Count:");
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::Text("%llu", static_cast<unsigned long long>(OETime::OEGetFrameCount()));
        
        ImGui::PushFont(OEEditorTheme::SemiBoldFont);
        ImGui::Text("FPS:");
        ImGui::PopFont();
        ImGui::SameLine();
        ImGui::Text("%f", dt > 0.0f ? 1.0f / dt : 0.0f);

        if (ImGui::CollapsingHeader("Camera Metadata"))
        {
            ImGui::PushFont(OEEditorTheme::SemiBoldFont);
            ImGui::Text("Current Camera (Pointer):");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%p", m_CurrentScene->GetCurrentCamera());


            EditorUtils::DrawMat4ReadOnly("Projection Matrix", m_CurrentScene->GetCurrentCamera().GetProjection());
            EditorUtils::DrawMat4ReadOnly("View Matrix", m_CurrentScene->GetCurrentCamera().GetView());
            EditorUtils::DrawMat4ReadOnly("View Projection Matrix", m_CurrentScene->GetCurrentCamera().GetViewProjection());

            if (ImGui::CollapsingHeader("Position"))
            {
                if (ImGui::BeginTable("CameraPositionTable", 3, ImGuiTableFlags_SizingStretchProp))
                {
                    ImGui::TableSetupColumn("X");
                    ImGui::TableSetupColumn("Y");
                    ImGui::TableSetupColumn("Z");

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::Button("X", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetPosition().x);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetPosition().y);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                    ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetPosition().z);

                    ImGui::EndTable();
                }
            }

            ImGui::Spacing();

            ImGui::PushFont(OEEditorTheme::SemiBoldFont);
            ImGui::Text("Yaw:");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%f", m_CurrentScene->GetCurrentCamera().GetYaw());
            ImGui::Spacing();

            ImGui::PushFont(OEEditorTheme::SemiBoldFont);
            ImGui::Text("Pitch:");
            ImGui::PopFont();
            ImGui::SameLine();
            ImGui::Text("%f", m_CurrentScene->GetCurrentCamera().GetPitch());
            ImGui::Spacing();

            if (ImGui::CollapsingHeader("Forward"))
            {
                if (ImGui::BeginTable("CameraForwardTable", 3, ImGuiTableFlags_SizingStretchProp))
                {
                    ImGui::TableSetupColumn("X");
                    ImGui::TableSetupColumn("Y");
                    ImGui::TableSetupColumn("Z");

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::Button("X", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetForward().x);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetForward().y);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                    ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetForward().z);

                    ImGui::EndTable();
                }
            }

            if (ImGui::CollapsingHeader("Right"))
            {
                if (ImGui::BeginTable("CameraRightTable", 3, ImGuiTableFlags_SizingStretchProp))
                {
                    ImGui::TableSetupColumn("X");
                    ImGui::TableSetupColumn("Y");
                    ImGui::TableSetupColumn("Z");

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::Button("X", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetRight().x);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetRight().y);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 1.0f, 1.0f));
                    ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetRight().z);

                    ImGui::EndTable();
                }
            }

            if (ImGui::CollapsingHeader("Up"))
            {
                if (ImGui::BeginTable("CameraUpTable", 3, ImGuiTableFlags_SizingStretchProp))
                {
                    ImGui::TableSetupColumn("X");
                    ImGui::TableSetupColumn("Y");
                    ImGui::TableSetupColumn("Z");

                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                    ImGui::Button("X", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetUp().x);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
                    ImGui::Button("Y", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("%.3f", m_CurrentScene->GetCurrentCamera().GetUp().y);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
                    ImGui::Button("Z", ImVec2(24.0f, 22.0f));
                    ImGui::PopStyleColor(2);
                    ImGui::SameLine();
                    ImGui::SetNextItemWidth(-1.0f);
                    ImGui::Text("% .3f", m_CurrentScene->GetCurrentCamera().GetUp().z);

                    ImGui::EndTable();
                }
            }
        }

        return true;
    }

    bool OESceneManagerPanel::DrawSceneManagerPanel()
    {
        if (ImGui::Begin("Scene Manager Panel"))
        {
            if (ImGui::CollapsingHeader("Metadata"))
                DrawSceneMetadata();
            
            DrawSceneEdit();
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            DrawSceneDeletion();
        }

        ImGui::End();
        return true;
    }
}