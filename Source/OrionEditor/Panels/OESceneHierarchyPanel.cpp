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

    bool OESceneHierarchyPanel::DrawCommandPalette()
    {
        m_OpenCommandPalette = true;
        if (m_OpenCommandPalette)
        {
            ImGui::OpenPopup("Command Palette");
            m_OpenCommandPalette = false; 
        }

        if (ImGui::BeginPopupModal("Command Palette", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            if (ImGui::BeginMenu("Add..."))
            {
                if (ImGui::MenuItem("Add Game Entity"))
                {
                    if (m_Registry->GetAllGameEntities().size() >= ECS::MAX_GAME_ENTITIES)
                        m_OESHPDataNeeded.CreationDataNeeded->OpenMaxGameEntitiesReached = true;

                    if (m_OESHPDataNeeded.CreationDataNeeded->OpenMaxGameEntitiesReached)
                    {
                        ImGui::OpenPopup("Max Game Entities Reached");
                        m_OESHPDataNeeded.CreationDataNeeded->OpenMaxGameEntitiesReached = false;
                    }

                    if (ImGui::BeginPopupModal("Max Game Entities Reached", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        ImGui::Text("Max Game Entites allowed in a Scene is %d, got %d", ECS::MAX_GAME_ENTITIES, static_cast<int>(m_Registry->GetAllGameEntities().size()));
                        if (ImGui::Button("OK"))
                        {
                            m_Registry->ResizeVectorToMax(); 
                            m_OESHPDataNeeded.CreationDataNeeded->OpenMaxGameEntitiesReached = true; // Reset
                            ImGui::CloseCurrentPopup();
                        }
                        ImGui::EndPopup();
                    }

                    // All checks passed, so we create a new entity
                    // Create popup for Game Entity name first
                    m_OESHPDataNeeded.CreationDataNeeded->OpenGameEntityNameDialog = true;
                    if (m_OESHPDataNeeded.CreationDataNeeded->OpenGameEntityNameDialog)
                    {
                        ImGui::OpenPopup("New Game Entity");
                        m_OESHPDataNeeded.CreationDataNeeded->OpenGameEntityNameDialog = false;
                    }

                    if (ImGui::BeginPopupModal("New Game Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                    {
                        // Now, get credentials from the user
                        ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBuffer, sizeof(m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBuffer));
                        
                        if (ImGui::Button("OK"))
                        {
                            std::string entityName = m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBuffer;
                            m_Registry->CreateGameEntity(entityName);
                            m_OESHPDataNeeded.CreationDataNeeded->GameEntityCreationSuccess = true; // success
                            
                            if (m_OESHPDataNeeded.CreationDataNeeded->GameEntityCreationSuccess)
                                ImGui::OpenPopup("Success");

                            if (ImGui::BeginPopupModal("Success", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                            {
                                ImGui::Text("Sucessfully created Game Entity %s.", entityName);
                                if (ImGui::Button("OK")) { ImGui::CloseCurrentPopup(); }
                                ImGui::EndPopup();
                            }
                            ImGui::CloseCurrentPopup();
                        }

                        ImGui::SameLine();
                        if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                        ImGui::EndPopup();
                    }

                }
                ImGui::EndMenu();

                if (ImGui::BeginMenu("Add Component..."))
                {
                    if (ImGui::MenuItem("Transform Component"))
                    {
                        m_OESHPDataNeeded.CreationDataNeeded->OpenNewTransformComponentDialog = true;
                        if (m_OESHPDataNeeded.CreationDataNeeded->OpenNewTransformComponentDialog)
                        {
                            ImGui::OpenPopup("New Transform Component");
                            m_OESHPDataNeeded.CreationDataNeeded->OpenNewTransformComponentDialog = false;
                        }

                        if (ImGui::BeginPopupModal("New Transform Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForTCCreation, sizeof(m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForTCCreation));
                            
                            if (ImGui::Button("OK"))
                            {
                                std::string gameEntityName = m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForTCCreation;
                                ECS::OEGameEntityID id = m_Registry->GetEntityIDByName(gameEntityName);

                                // Default properties
                                ECS::OETransformComponent type;
                                type.Position = { 0.0f, 0.0f, 0.0f };
                                type.Rotation = { 0.0f, 0.0f, 0.0f };
                                type.Scale = { 1.0f, 1.0f, 1.0f };

                                m_Registry->TransformComponent.AddComponent(id, type);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                            ImGui::EndPopup();
                        }
                    }

                    if (ImGui::MenuItem("Physics Component"))
                    {
                        m_OESHPDataNeeded.CreationDataNeeded->OpenNewPhysicsComponentDialog = true;
                        if (m_OESHPDataNeeded.CreationDataNeeded->OpenNewPhysicsComponentDialog)
                        {
                            ImGui::OpenPopup("New Physics Component");
                            m_OESHPDataNeeded.CreationDataNeeded->OpenNewPhysicsComponentDialog = false;
                        }

                        if (ImGui::BeginPopupModal("New Physics Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForPCCreation, sizeof(m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForPCCreation));

                            if (ImGui::Button("OK"))
                            {
                                std::string gameEntityName = m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForPCCreation;
                                ECS::OEGameEntityID id = m_Registry->GetEntityIDByName(gameEntityName);
                                
                                ECS::OEPhysicsComponent type; // properites already set
                                m_Registry->PhysicsComponent.AddComponent(id, type);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                            ImGui::EndPopup();
                        }
                    }

                    if (ImGui::MenuItem("Renderable Component"))
                    {
                        m_OESHPDataNeeded.CreationDataNeeded->OpenNewRenderableComponentDialog = true;
                        if (m_OESHPDataNeeded.CreationDataNeeded->OpenNewRenderableComponentDialog)
                        {
                            ImGui::OpenPopup("New Renderable Component");
                            m_OESHPDataNeeded.CreationDataNeeded->OpenNewRenderableComponentDialog = false;
                        }

                        if (ImGui::BeginPopupModal("New Renderable Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForRCCreation, sizeof(m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForRCCreation));

                            if (ImGui::Button("OK"))
                            {
                                std::string gameEntityName = m_OESHPDataNeeded.CreationDataNeeded->GameEntityNameBufferForRCCreation;
                                ECS::OEGameEntityID id = m_Registry->GetEntityIDByName(gameEntityName);

                                ECS::OERenderableComponent type;
                                type.b_Visible = true; 

                                m_Registry->RenderableComponent.AddComponent(id, type);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                            ImGui::EndPopup();
                        }
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Delete..."))
                {
                    if (ImGui::MenuItem("Game Entity"))
                    {
                        m_OESHPDataNeeded.DeletionDataNeeded->OpenGameEntityDeletionDialog = true;
                        if (m_OESHPDataNeeded.DeletionDataNeeded->OpenGameEntityDeletionDialog)
                        {
                            ImGui::OpenPopup("Game Entity Deletion");
                            m_OESHPDataNeeded.DeletionDataNeeded->OpenGameEntityDeletionDialog = false;
                        }

                        if (ImGui::BeginPopupModal("Game Entity Deletion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameBufferToDelete, sizeof(m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameBufferToDelete));

                            if (ImGui::Button("OK"))
                            {
                                std::string gameEntityName = m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameBufferToDelete;
                                ECS::OEGameEntityID id = m_Registry->GetEntityIDByName(gameEntityName);
                                m_Registry->DeleteGameEntity(id);
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                            ImGui::EndPopup();
                        }
                    }

                    if (ImGui::MenuItem("Component"))
                    {
                        m_OESHPDataNeeded.DeletionDataNeeded->OpenComponentDeletionDialog = true;
                        if (m_OESHPDataNeeded.DeletionDataNeeded->OpenComponentDeletionDialog)
                        {
                            ImGui::OpenPopup("Component Deletion");
                            m_OESHPDataNeeded.DeletionDataNeeded->OpenComponentDeletionDialog = false;
                        }

                        if (ImGui::BeginPopupModal("Component Deletion", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
                        {
                            // Component Types
                            const char* componentTypes[] = {
                                "Transform Component", "Physics Component", "Renderable Component", "Camera Component"
                            };

                            if (ImGui::BeginCombo("Component Types", componentTypes[m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex]))
                            {
                                for (int n = 0; n < IM_ARRAYSIZE(componentTypes); n++)
                                {
                                    bool b_IsComponentTypeSelected = (m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex == n);
                                    if (ImGui::Selectable(componentTypes[n], b_IsComponentTypeSelected))
                                        m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex = n;

                                    if (b_IsComponentTypeSelected)
                                        ImGui::SetItemDefaultFocus();
                                }
                                ImGui::EndCombo();
                            }

                            ImGui::InputText("Game Entity Name...", m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameInWhichComponentWillBeDeleted, sizeof(m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameInWhichComponentWillBeDeleted));
                            
                            if (ImGui::Button("OK"))
                            {
                                std::string name = m_OESHPDataNeeded.DeletionDataNeeded->GameEntityNameInWhichComponentWillBeDeleted;
                                ECS::OEGameEntityID id = m_Registry->GetEntityIDByName(name);
                                
                                if (m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex == 0)
                                    m_Registry->TransformComponent.DeleteComponent(id);
                                else if (m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex == 1)
                                    m_Registry->PhysicsComponent.DeleteComponent(id);
                                else if (m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex == 2)
                                    m_Registry->RenderableComponent.DeleteComponent(id);
                                else if (m_OESHPDataNeeded.DeletionDataNeeded->ComponentDeletionIndex == 3)
                                    m_Registry->CameraComponent.DeleteComponent(id);
                                else; // do nothing (lol)
                                ImGui::CloseCurrentPopup();
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
                            ImGui::EndPopup();
                        }
                    }
                }
            }

            if (ImGui::Button("Close")) { ImGui::CloseCurrentPopup(); }
            ImGui::EndPopup();
        }

        return true;
    }

    bool OESceneHierarchyPanel::DrawSceneHierarchyPanel()
    {
        ImGui::Begin("Scene Hierachy");

        ShowSceneHierachy();

        if (ImGui::Button("Command Palette"))
            DrawCommandPalette();

        ImGui::End();
        return true;
    }
}