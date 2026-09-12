#include "OECommandPanel.h"

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
	bool OECommandPanel::ShowEntityCommands()
	{
		if (!m_Registry || !m_EditorContext)
			return false;

		if (ImGui::CollapsingHeader("Game Entity Commands"))
		{
			if (ImGui::Button("Add Game Entity"))
			{
				ImGui::OpenPopup("New Game Entity");
			}

			if (ImGui::BeginPopupModal("New Game Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("Name...", m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBuffer,
					sizeof(m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBuffer));

				if (ImGui::Button("Create"))
				{
					std::string st_GameEntityName = m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBuffer;
					m_Registry->CreateGameEntity(st_GameEntityName);
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}

			ImGui::Spacing();

			if (ImGui::Button("Delete Game Entity"))
			{
				ECS::OEGameEntityID id = m_EditorContext->SelectedEntity;

				if (id != ECS::INVALID_GAME_ENTITY_ID)
				{
					m_Registry->DeleteGameEntity(id);
					m_EditorContext->SelectedEntity = ECS::INVALID_GAME_ENTITY_ID;
				}
				else
					ImGui::OpenPopup("Entity Not Selected");
			}

			if (ImGui::Button("Rename Game Entity"))
			{
				if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
					ImGui::OpenPopup("Rename Game Entity");
				else
					ImGui::OpenPopup("Entity Not Selected");
			}

			if (ImGui::BeginPopupModal("Rename Game Entity", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::InputText("Name...", m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBufferForRenaming,
					sizeof(m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBufferForRenaming));

				if (ImGui::Button("OK"))
				{
					std::string st_GameEntityName = m_OECPDataNeeded.GameEntityActionDataNeeded.GameEntityNameBufferForRenaming;
					ECS::OEGameEntityID id = m_EditorContext->SelectedEntity;

					if (id != ECS::INVALID_GAME_ENTITY_ID)
					{
						m_Registry->SetEntityName(id, st_GameEntityName);
						ImGui::CloseCurrentPopup();
					}
					else
						ImGui::OpenPopup("Entity Not Selected");
				}

				if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }

				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupModal("Entity Not Selected", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Entity is not selected!");
				if (ImGui::Button("OK")) { ImGui::CloseCurrentPopup(); }
				ImGui::EndPopup();
			}
		}

		return true;
	}

	bool OECommandPanel::ShowComponentCommands()
	{
		if (!m_Registry || !m_EditorContext)
			return false; 

		if (ImGui::CollapsingHeader("Component Commands"))
		{
			const char* components[] = {
				"Transform Component", "Physics Component", "Renderable Component", "Camera Component"
			};

			if (ImGui::BeginCombo("Components", components[m_OECPDataNeeded.ComponentActionDataNeeded.ComponentTypesIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(components); n++)
				{
					bool b_IsComponentTypeSelected = (m_OECPDataNeeded.ComponentActionDataNeeded.ComponentTypesIndex == n);
					if (ImGui::Selectable(components[n], b_IsComponentTypeSelected))
						m_OECPDataNeeded.ComponentActionDataNeeded.ComponentTypesIndex = n;

					if (b_IsComponentTypeSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			switch (m_OECPDataNeeded.ComponentActionDataNeeded.ComponentTypesIndex)
			{
				case 0:
				{
					if (ImGui::Button("Create Component"))
					{
						ECS::OETransformComponent transform;
						transform.Position = { 0.0f, 0.0f, 0.0f };
						transform.Rotation = { 0.0f, 0.0f, 0.0f };
						transform.Scale = { 1.0f, 1.0f, 1.0f };

						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->TransformComponent.AddComponent(m_EditorContext->SelectedEntity, transform);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					if (ImGui::Button("Delete Component"))
					{
						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->TransformComponent.DeleteComponent(m_EditorContext->SelectedEntity);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					break;
				}

				case 1:
				{
					if (ImGui::Button("Create Component"))
					{
						ECS::OEPhysicsComponent physics;
						physics.Mass = 0.0f;
						physics.Radius = 0.0f;
						physics.LinearDrag = 0.0f;
						physics.Velocity = { 0.0f, 0.0f, 0.0f };
						physics.Accleration = { 0.0f, 0.0f, 0.0f };
						physics.IsStatic = true;

						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->PhysicsComponent.AddComponent(m_EditorContext->SelectedEntity, physics);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					if (ImGui::Button("Delete Component"))
					{
						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->PhysicsComponent.DeleteComponent(m_EditorContext->SelectedEntity);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					break;
				}

				case 2:
				{
					if (ImGui::Button("Create Component"))
					{
						ECS::OERenderableComponent render;
						render.b_Visible = true;

						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->RenderableComponent.AddComponent(m_EditorContext->SelectedEntity, render);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					if (ImGui::Button("Delete Component"))
					{
						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->RenderableComponent.DeleteComponent(m_EditorContext->SelectedEntity);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					break;
				}

				case 3:
				{
					if (ImGui::Button("Create Component"))
					{
						ECS::OECameraComponent camera;
						OrionRenderer::ORCamera Camera = OrionRenderer::ORCamera(0.0f, 0.0f, 0.0f, 0.0f);
						camera.Camera = Camera;
						camera.FarClip = 0.0f;
						camera.NearClip = 0.0f;
						camera.Primary = false;
						
						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->CameraComponent.AddComponent(m_EditorContext->SelectedEntity, camera);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					if (ImGui::Button("Delete Component"))
					{
						if (m_EditorContext->SelectedEntity != ECS::INVALID_GAME_ENTITY_ID)
							m_Registry->CameraComponent.DeleteComponent(m_EditorContext->SelectedEntity);
						else
							ImGui::OpenPopup("Entity Not Selected");
					}

					break;
				}
			}
		}

		return true;
	}

	bool OECommandPanel::DrawOECommandPanel()
	{
		if (ImGui::Begin("Command Panel"))
		{
			ShowEntityCommands();
			
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ShowComponentCommands();
		}

		ImGui::End();
		return true;
	}
}