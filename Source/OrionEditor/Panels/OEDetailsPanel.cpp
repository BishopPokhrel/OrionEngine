#include "OEDetailsPanel.h"

#include <OrionEditor/UI/OEEditorTheme.h>

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
	bool OEDetailsPanel::DrawEntityDetails()
	{
		if (m_EditorContext->SelectedEntity == ECS::INVALID_GAME_ENTITY_ID)
		{
			ImGui::TextDisabled("No entities selected yet.");
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.000f, 1.000f, 1.000f, 0.300));
			ImGui::TextWrapped("Try selecting entities in the Scene Hierarchy Panel to inspect its properties here");
			ImGui::PopStyleColor();
			return false;
		}

		const ECS::OEGameEntityID entityID = m_EditorContext->SelectedEntity;
		const std::string entityName = m_Registry->GetEntityNameByID(entityID);

		ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.000f, 0.000f, 0.091f, 1.000f));
		ImGui::BeginChild("EntityHeader", ImVec2(0.0f, 82.0f), true);

		ImGui::PushFont(OEEditorTheme::SemiBoldFont);
		ImGui::TextColored(ImVec4(0.423f, 0.149f, 0.652f, 0.726f), "GAME ENTITY");
		ImGui::PopFont();
		ImGui::SameLine();
		ImGui::TextDisabled("| ID: %llu", static_cast<unsigned long long>(entityID));

		if (ImGui::InputText("##EntityName", m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer, sizeof(m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer)))
			m_Registry->SetEntityName(m_EditorContext->SelectedEntity, std::string(m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer));

		ImGui::EndChild();
		ImGui::PopStyleColor();

		if (ImGui::CollapsingHeader("Transform Component"))
		{
			if (ECS::OETransformComponent* transform = m_Registry->TransformComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				if (ImGui::BeginTable("PositionTable", 4, ImGuiTableFlags_SizingStretchProp))
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
					ImGui::DragFloat("##PositionX", &transform->Position.x, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Button("Y", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##PositionY", &transform->Position.y, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(3);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::Button("Z", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##PositionZ", &transform->Position.z, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("RotationTable", 4, ImGuiTableFlags_SizingStretchProp))
				{
					ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("Y");
					ImGui::TableSetupColumn("Z");

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Rotation");

					ImGui::TableSetColumnIndex(1);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Button("X", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##RotationX", &transform->Rotation.x, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Button("Y", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##RotationY", &transform->Rotation.y, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(3);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::Button("Z", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##RotationZ", &transform->Rotation.z, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::EndTable();
				}

				if (ImGui::BeginTable("ScaleTable", 4, ImGuiTableFlags_SizingStretchProp))
				{
					ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("Y");
					ImGui::TableSetupColumn("Z");

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Scale");

					ImGui::TableSetColumnIndex(1);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Button("X", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##ScaleX", &transform->Scale.x, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Button("Y", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##ScaleY", &transform->Scale.y, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(3);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::Button("Z", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##ScaleZ", &transform->Scale.z, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::EndTable();
				}
			}
		}

		if (ImGui::CollapsingHeader("Physics Component"))
		{
			if (ECS::OEPhysicsComponent* physics = m_Registry->PhysicsComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				
				ImGui::Text("Mass");
				ImGui::SameLine(100.0f);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##Mass: ", &physics->Mass, 0.1f, 0.0f, 0.0f, "%.2f");

				// Velocity
				if (ImGui::BeginTable("VelocityTable", 4, ImGuiTableFlags_SizingStretchProp))
				{
					ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("Y");
					ImGui::TableSetupColumn("Z");

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Velocity");

					ImGui::TableSetColumnIndex(1);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Button("X", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##VelocityX", &physics->Velocity.x, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Button("Y", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##VelocityY", &physics->Velocity.y, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(3);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::Button("Z", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##VelocityZ", &physics->Velocity.z, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::EndTable();
				}

				// Acceleration

				if (ImGui::BeginTable("AccelerationTable", 4, ImGuiTableFlags_SizingStretchProp))
				{
					ImGui::TableSetupColumn("Property", ImGuiTableColumnFlags_WidthFixed, 75.0f);
					ImGui::TableSetupColumn("X");
					ImGui::TableSetupColumn("Y");
					ImGui::TableSetupColumn("Z");

					ImGui::TableNextRow();

					ImGui::TableSetColumnIndex(0);
					ImGui::Text("Acceleration");

					ImGui::TableSetColumnIndex(1);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
					ImGui::Button("X", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##AccelerationX", &physics->Accleration.x, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(2);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 1.0f, 0.0f, 1.0f));
					ImGui::Button("Y", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##AccelerationY", &physics->Accleration.y, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::TableSetColumnIndex(3);
					ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
					ImGui::Button("Z", ImVec2(24.0f, 22.0f));
					ImGui::PopStyleColor(2);
					ImGui::SameLine();
					ImGui::SetNextItemWidth(-1.0f);
					ImGui::DragFloat("##AccelerationZ", &physics->Accleration.z, 0.1f, 0.0f, 0.0f, "%.2f");

					ImGui::EndTable();
				}

				ImGui::Text("Is Static");
				ImGui::SameLine(100.0f);
				ImGui::Checkbox("##IsStatic", &physics->IsStatic);

				ImGui::Text("Linear Drag");
				ImGui::SameLine(100.0f);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##LinearDrag", &physics->LinearDrag, 0.1f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("Radius");
				ImGui::SameLine(100.0f);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##Radius", &physics->Radius, 0.1f, 0.0f,0.0f, "%.2f");
			}
		}

		if (ImGui::CollapsingHeader("Renderable Component"))
		{
			if (ECS::OERenderableComponent* render = m_Registry->RenderableComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				ImGui::Text("Is Visible");
				ImGui::SameLine(100.0f);
				ImGui::Checkbox("##IsVisible", &render->b_Visible);
			}
		}

		if (ImGui::CollapsingHeader("Camera Component"))
		{
			if (ECS::OECameraComponent* camera = m_Registry->CameraComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				ImGui::Text("Primary");
				ImGui::SameLine(100.0f);
				ImGui::Checkbox("##Primary", &camera->Primary);

				ImGui::Text("Near Clip");
				ImGui::SameLine(100.0f);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##NearClip", &camera->NearClip, 0.1f, 0.0f, 0.0f, "%.2f");

				ImGui::Text("Far Clip");
				ImGui::SameLine(100.0f);
				ImGui::SetNextItemWidth(-1.0f);
				ImGui::DragFloat("##FarClip", &camera->FarClip, 0.1f, 0.0f, 0.0f, "%.2f");
			}
		}

		if (ImGui::Button("+ Add New Component"))
			ImGui::OpenPopup("New Component");

		if (ImGui::BeginPopupModal("New Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Let the user choose the type of component
			// Options
			const char* componentTypeOptions[] = {
				"Transform Component", "Physics Component", "Renderable Component", "Camera Component"
			};

			if (ImGui::BeginCombo("Component Type", componentTypeOptions[m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(componentTypeOptions); n++)
				{
					bool b_IsComponentTypeSelected = (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == n);
					if (ImGui::Selectable(componentTypeOptions[n], b_IsComponentTypeSelected))
						m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex = n;

					if (b_IsComponentTypeSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("OK"))
			{
				if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 0)
				{
					if (!m_Registry->HasComponent<ECS::OETransformComponent>(m_EditorContext->SelectedEntity))
					{
						ECS::OETransformComponent transform;
						transform.Position = { 0.0f, 0.0f, 0.0f };
						transform.Rotation = { 0.0f, 0.0f, 0.0f };
						transform.Scale = { 1.0f, 1.0f, 1.0f };

						m_Registry->TransformComponent.AddComponent(m_EditorContext->SelectedEntity, transform);
					}
					else
						ImGui::Text("Entity %s with ID %llu already has a Transform Component!", entityName.c_str(), static_cast<unsigned long long>(m_EditorContext->SelectedEntity));
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 1)
				{
					if (!m_Registry->HasComponent<ECS::OEPhysicsComponent>(m_EditorContext->SelectedEntity))
					{
						ECS::OEPhysicsComponent physics;

						// All properties already set, no need to set them now like in transform component
						m_Registry->PhysicsComponent.AddComponent(m_EditorContext->SelectedEntity, physics);
					}
					else
						ImGui::Text("Entity %s with ID %llu already has a Physics Component!", entityName.c_str(), static_cast<unsigned long long>(m_EditorContext->SelectedEntity));
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 2)
				{
					if (!m_Registry->HasComponent<ECS::OERenderableComponent>(m_EditorContext->SelectedEntity))
					{
						ECS::OERenderableComponent render;
						render.b_Visible = true;
						OEEditorRenderCommands::InitializeRenderableRemaining(render);

						// Other propeties are smart pointers (Ref: std::shared_ptr) so we cannot exactly set the default properties smoothly
						// until they are set by the renderer, render system, or scene renderer
						// the editor should NOT know anything about the renderer or about the renderer pipeline
						// or architecure. It may know about the ECS, it may request creation, deletion, editon of game entities and components
						// but it should NOT know anything about how those things are rendered. 
						// Rendering is low level. The editor, which sits on top of the engine, uses the engine API, and does not use the renderer API
						// The engine only uses the renderer API to render all entities in a given scene
						m_Registry->RenderableComponent.AddComponent(m_EditorContext->SelectedEntity, render);
					}
					else
						ImGui::Text("Entity %s with ID %llu already has a Renderable Component!", entityName.c_str(), static_cast<unsigned long long>(m_EditorContext->SelectedEntity));
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 3)
				{
					if (!m_Registry->HasComponent<ECS::OECameraComponent>(m_EditorContext->SelectedEntity))
					{
						ECS::OECameraComponent cameraComp;

						// Camera property already set (see OEComponen.h)
						cameraComp.NearClip = 0.1f;
						cameraComp.FarClip = 1000.0f;
						cameraComp.Primary = true;

						m_Registry->CameraComponent.AddComponent(m_EditorContext->SelectedEntity, cameraComp);
					}
					else
						ImGui::Text("Entity %s with ID %llu already has a Camera Component!", entityName.c_str(), static_cast<unsigned long long>(m_EditorContext->SelectedEntity));
				}
				ImGui::CloseCurrentPopup();

				// cleanup after clsoing popup
				m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex = 0;
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		if (ImGui::Button("Delete Component"))
			ImGui::OpenPopup("Delete Component");

		if (ImGui::BeginPopupModal("Delete Component", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			// Let the user choose the type of component they want to delete
			// Options
			const char* componentTypeOptions[] = {
				"Transform Component", "Physics Component", "Renderable Component", "Camera Component"
			};

			if (ImGui::BeginCombo("Component Type", componentTypeOptions[m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex]))
			{
				for (int n = 0; n < IM_ARRAYSIZE(componentTypeOptions); n++)
				{
					bool b_IsComponentTypeSelected = (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex == n);
					if (ImGui::Selectable(componentTypeOptions[n], b_IsComponentTypeSelected))
						m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex = n;

					if (b_IsComponentTypeSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("OK"))
			{
				if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex == 0)
					m_Registry->TransformComponent.DeleteComponent(m_EditorContext->SelectedEntity);
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex == 1)
					m_Registry->PhysicsComponent.DeleteComponent(m_EditorContext->SelectedEntity);
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex == 2)
					m_Registry->RenderableComponent.DeleteComponent(m_EditorContext->SelectedEntity);
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex == 3)
					m_Registry->CameraComponent.DeleteComponent(m_EditorContext->SelectedEntity);
				ImGui::CloseCurrentPopup();

				// Cleanup after closing popup
				m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeDeletionIndex = 0; 
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		return true;
	}

	bool OEDetailsPanel::DrawDetailsPanel()
	{
		ImGui::Begin("Details");

		DrawEntityDetails();

		ImGui::End();
		return true;
	}
}