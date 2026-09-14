#include "OEImGuiLayer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace OrionEngine::OrionEditor
{
	bool OEImGuiLayer::InitImGui(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); 
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330 core");

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

	bool OEImGuiLayer::DrawViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer)
	{
		m_Viewport.ShowViewport(framebuffer);
		return true;
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

		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
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