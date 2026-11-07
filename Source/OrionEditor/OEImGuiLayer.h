#pragma once
#define GLFW_INCLUDE_NONE

#include "Panels/OEDetailsPanel.h"
#include "Panels/OESceneHierarchyPanel.h"
#include "Panels/OESceneManagerPanel.h"
#include "Panels/OECommandPanel.h"
#include "Panels/OEResourceBrowserPanel.h"
#include "Panels/OEConsolePanel.h"

#include "EditorSpecificRendering/OEGridRenderer.h"

#include "OEViewport.h"

#include <optional>
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
		bool ShowResourceBrowserPanel(AssetManagement::OEAssetManager* assetManager, OEScene* scene);
		bool ShowConsolePanel();

		bool DrawViewport(const Ref<OrionRenderer::ORFrameBuffer>& framebuffer, OEScene* scene, AssetManagement::OEAssetManager* assetManager);
		bool ShowViewportGrid(OEScene* scene);

		bool CreateDockspace();

		void CreateMenuBar(OEScene* scene);

	private:

		struct OEMainMenuBarDataNeeded
		{
			char ProjectNameBuffer[1024];
			bool OpenProjectNamePopup;
			bool OpenSceneFilePopup;
			bool OpenProjectFilePopup;
		};

	private:

		Scope<OEEditorContext> m_EditorContext = CreateScope<OEEditorContext>();

		OEDetailsPanel m_DetailsPanel;
		OESceneHierarchyPanel m_SceneHierarchyPanel;
		OESceneManagerPanel m_SceneManagerPanel;
		std::optional<OEResourceBrowserPanel> m_ResourceBrowserPanel;
		OECommandPanel m_CommandPanel;
		OEConsolePanel m_ConsolePanel; 

		std::optional<OEViewport> m_Viewport;
		EditorSpecificRendering::OEGridRenderer m_GridRenderer;

		OEMainMenuBarDataNeeded m_OEMMBDN;
	};
}