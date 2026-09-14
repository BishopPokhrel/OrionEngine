#pragma once

#include "Panels/OEDetailsPanel.h"
#include "Panels/OESceneHierarchyPanel.h"
#include "Panels/OESceneManagerPanel.h"
#include "Panels/OECommandPanel.h"

#include "OEViewport.h"

#include <GLFW/glfw3.h>

namespace OrionEngine::OrionEditor
{
	class OEImGuiLayer
	{
	public:

		OEImGuiLayer()
			: m_SceneHierarchyPanel(m_EditorContext.get()), m_DetailsPanel(m_EditorContext.get()), 
			m_CommandPanel(m_EditorContext.get())
		{ }

		bool InitImGui(GLFWwindow* window);
		bool ShutdownImGui();

		bool StartImGuiNewFrame();
		bool RenderImGui();

		bool ShowSceneHierarchyPanel(OEScene* scene);
		bool ShowDetailsPanel(OEScene* scene);
		bool ShowSceneManagerPanel(OEScene* scene);
		bool ShowCommandPanel(OEScene* scene);
		bool DrawViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer);

		bool CreateDockspace();

	private:

		Scope<OEEditorContext> m_EditorContext = CreateScope<OEEditorContext>();

		OEDetailsPanel m_DetailsPanel;
		OESceneHierarchyPanel m_SceneHierarchyPanel;
		OESceneManagerPanel m_SceneManagerPanel;
		OECommandPanel m_CommandPanel;

		OEViewport m_Viewport;
	};
}