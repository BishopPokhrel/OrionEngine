#pragma once

#include "Panels/OEDetailsPanel.h"
#include "Panels/OESceneHierarchyPanel.h"
#include "Panels/OESceneManagerPanel.h"

#include <GLFW/glfw3.h>

namespace OrionEngine::OrionEditor
{
	class OEImGuiLayer
	{
	public:

		bool InitImGui(GLFWwindow* window);
		bool ShutdownImGui();

		bool StartImGuiNewFrame();
		bool RenderImGui();

		bool ShowSceneHierarchyPanel();
		bool ShowDetailsPanel();
		bool ShowSceneManagerPanel();

		bool CreateDockspace();

	private:

		OEDetailsPanel m_DetailsPanel;
		OESceneHierarchyPanel m_SceneHierarchyPanel;
		OESceneManagerPanel m_SceneManagerPanel;
	};
}