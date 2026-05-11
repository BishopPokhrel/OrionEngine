#include "OEDetailsPanel.h"

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
	bool OEDetailsPanel::DrawEntityDetails()
	{
		if (m_EditorContext->SelectedEntity == ECS::INVALID_GAME_ENTITY_ID)
		{
			ImGui::Text("No entities selected yet.");
			return false;
		}

		ImGui::Text("Game Entity Name: "); 
		ImGui::SameLine();

		std::string entityName = m_Registry->GetEntityNameByID(m_EditorContext->SelectedEntity);
		if (ImGui::InputText(entityName.c_str(), m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer, sizeof(m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer)))
			m_Registry->SetEntityName(m_EditorContext->SelectedEntity, std::string(m_OEDPDataNeeded.GameEntityDataNeeded->GameEntityNameBuffer));

		ImGui::Text("Game Entity ID: %d", m_EditorContext->SelectedEntity); 

		if (ImGui::CollapsingHeader("Transform Component"))
		{
			if (ECS::OETransformComponent* transform = m_Registry->TransformComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				ImGui::DragFloat3("Position: ", &transform->Position[0], 0.1f);
				ImGui::DragFloat3("Rotation: ", &transform->Rotation[0], 0.1f);
				ImGui::DragFloat3("Scale: ", &transform->Scale[0], 0.1f);
			}
		}

		if (ImGui::CollapsingHeader("Physics Component"))
		{
			if (ECS::OEPhysicsComponent* physics = m_Registry->PhysicsComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				ImGui::DragFloat("Mass: ", &physics->Mass, 0.1f);
				ImGui::DragFloat3("Velocity: ", &physics->Velocity[0], 0.1f);
				ImGui::DragFloat3("Accleration: ", &physics->Accleration[0], 0.1f);
				if (ImGui::Checkbox("Is Static: ", &physics->IsStatic)) {}
				ImGui::DragFloat("Linear Drag: ", &physics->LinearDrag, 0.1f);
				ImGui::DragFloat("Radius: ", &physics->Radius, 0.1f);
			}
		}

		if (ImGui::CollapsingHeader("Renderable Component"))
		{
			if (ECS::OERenderableComponent* render = m_Registry->RenderableComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				if (ImGui::Checkbox("Is Visible", &render->b_Visible)) {}
			}
		}

		if (ImGui::CollapsingHeader("Camera Component"))
		{
			if (ECS::OECameraComponent* camera = m_Registry->CameraComponent.GetComponent(m_EditorContext->SelectedEntity))
			{
				if (ImGui::Checkbox("Primary: ", &camera->Primary)) {}
				ImGui::DragFloat("Near Clip: ", &camera->NearClip, 0.1f);
				ImGui::DragFloat("Far Clip: ", &camera->FarClip, 0.1f);
			}
		}

		if (ImGui::Button("Add New Component"))
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
					ECS::OETransformComponent transform;
					transform.Position = { 0.0f, 0.0f, 0.0f };
					transform.Rotation = { 0.0f, 0.0f, 0.0f };
					transform.Scale = { 1.0f, 1.0f, 1.0f };

					m_Registry->TransformComponent.AddComponent(m_EditorContext->SelectedEntity, transform);
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 1)
				{
					ECS::OEPhysicsComponent physics;

					// All properties already set, no need to set them now like in transform component
					m_Registry->PhysicsComponent.AddComponent(m_EditorContext->SelectedEntity, physics);
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 2)
				{
					ECS::OERenderableComponent render;
					render.b_Visible = true;

					// Other propeties are smart pointers (Ref: std::shared_ptr) so we cannot exactly set the default properties smoothly
					// until they are set by the renderer, render system, or scene renderer
					// the editor should NOT know anything about the renderer or about the renderer pipeline
					// or architecure. It may know about the ECS, it may request creation, deletion, editon of game entities and components
					// but it should NOT know anything about how those things are rendered. 
					// Rendering is low level. The editor, which sits on top of the engine, uses the engine API, and does not use the renderer API
					// The engine only uses the renderer API to render all entities in a given scene
					m_Registry->RenderableComponent.AddComponent(m_EditorContext->SelectedEntity, render);
				}
				else if (m_OEDPDataNeeded.ComponentDataNeeded->ComponentTypeCreationIndex == 3)
				{
					ECS::OECameraComponent cameraComp;

					// Camera property already set (see OEComponen.h)
					cameraComp.NearClip = 0.1f;
					cameraComp.FarClip = 1000.0f;
					cameraComp.Primary = true;

					m_Registry->CameraComponent.AddComponent(m_EditorContext->SelectedEntity, cameraComp);
				}
				else; // do nothing (lol)
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
				else; // do nothing (lol)
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
		ImGui::Begin("Details Panel");

		DrawEntityDetails();

		ImGui::End();
		return true;
	}
}