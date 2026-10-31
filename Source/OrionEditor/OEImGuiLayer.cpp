#include "OEImGuiLayer.h"
#include "UI/OEEditorTheme.h"

#include <ImGuizmo.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#if defined(_WIN64)
#include <OrionEngine/OS/Windows/WindowsOSAPI.h>
#endif

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine::OrionEditor
{
	bool OEImGuiLayer::InitImGui(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		OEEditorTheme::ApplyTheme();
		OEEditorTheme::LoadFonts();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

		m_GridRenderer.Init();

		return true;
	}

	bool OEImGuiLayer::ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return true;
	}

	bool OEImGuiLayer::StartImGuiNewFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		return true;
	}

	bool OEImGuiLayer::RenderImGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		return true;
	}

	bool OEImGuiLayer::ShowSceneHierarchyPanel(OEScene* scene)
	{
		m_SceneHierarchyPanel.SetRegistry(scene->GetRegistry());
		m_SceneHierarchyPanel.DrawSceneHierarchyPanel();
		return true;
	}

	bool OEImGuiLayer::ShowDetailsPanel(OEScene* scene)
	{
		m_DetailsPanel.SetRegistry(scene->GetRegistry());
		m_DetailsPanel.DrawDetailsPanel();
		return true;
	}

	bool OEImGuiLayer::ShowSceneManagerPanel(OEScene* scene)
	{
		m_SceneManagerPanel.SetScene(scene);
		m_SceneManagerPanel.DrawSceneManagerPanel();
		return true;
	}

	bool OEImGuiLayer::ShowCommandPanel(OEScene* scene)
	{
		m_CommandPanel.SetRegistry(scene->GetRegistry());
		m_CommandPanel.DrawOECommandPanel();
		return true;
	}

	bool OEImGuiLayer::DrawViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer, OEScene* scene)
	{
		m_Viewport.ShowViewport(framebuffer, scene);
		return true;
	}

	bool OEImGuiLayer::ShowViewportGrid(OEScene* scene)
	{
		m_GridRenderer.RenderGrid(scene->GetCurrentCamera());
		return true;
	}

	void OEImGuiLayer::CreateMenuBar(OEScene* scene)
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Create New Project..."))
					m_OEMMBDN.OpenProjectNamePopup = true;

				ImGui::Separator();

				if (ImGui::MenuItem("Save Scene...", "Ctrl+S"))
					scene->SerializeScene();

				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))
					m_OEMMBDN.OpenSceneFilePopup = true;

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (m_OEMMBDN.OpenProjectNamePopup)
		{
			ImGui::OpenPopup("Create New Project");
			m_OEMMBDN.OpenProjectNamePopup = false;
		}

		if (ImGui::BeginPopupModal("Create New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::InputText("Project Name...", m_OEMMBDN.ProjectNameBuffer, sizeof(m_OEMMBDN.ProjectNameBuffer));

			if (ImGui::Button("Create"))
			{
				std::string projectName = m_OEMMBDN.ProjectNameBuffer;
				scene->CreateProjectDir(projectName);
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();
			if (ImGui::Button("Cancel")) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		if (m_OEMMBDN.OpenSceneFilePopup)
		{
			m_OEMMBDN.OpenSceneFilePopup = false;

			const std::filesystem::path scenePath = OS::OpenWindowsFileDialog(L"Orion Scene (*.oescene)\0*.oescene\0All Files (*.*)\0*.*\0\0");
			if (!scenePath.empty())
				scene->DeserializeScene(scenePath);
		}
	}

	bool OEImGuiLayer::CreateDockspace()
	{
		ImGuiWindowFlags windowFlags =
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();

		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("DockSpaceWindow", nullptr, windowFlags);

		ImGui::PopStyleVar(2);

		ImGuiID dockspaceID = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspaceID, ImVec2(0, 0));
		ImGui::End();
		return true;
	}
}