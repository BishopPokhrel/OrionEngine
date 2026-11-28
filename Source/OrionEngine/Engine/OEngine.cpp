#include "OEngine.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/AssetManagement/AssetLoaders/OEMeshLoader.h>
#include <OrionEngine/Core/AssetManagement/AssetLoaders/OEMaterialLoader.h>
#include <OrionEngine/Core/AssetManagement/AssetLoaders/OETextureLoader.h>

namespace OrionEngine
{
	void OEngine::OInitEngineSubsystems()
	{
		m_Renderer = CreateScope<OrionRenderer::ORRenderer>();
		m_InputSystem = CreateScope<OEInputSystem>();
		m_AssetManager = CreateScope<AssetManagement::OEAssetManager>();
		m_Scene = CreateScope<OEScene>(m_AssetManager.get());

		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		OE_CORE_ASSERT(m_InputSystem, "Input System is null!");
		OE_CORE_ASSERT(m_AssetManager, "Asset Manager is null!");
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		m_Renderer->Init(OrionRenderer::ORGraphicsAPI::OpenGL);
		m_AssetManager->RegisterLoader(AssetManagement::OEAssetType::Mesh, CreateScope<AssetManagement::OEMeshLoader>());
		m_AssetManager->RegisterLoader(AssetManagement::OEAssetType::Material, CreateScope<AssetManagement::OEMaterialLoader>());
		m_AssetManager->RegisterLoader(AssetManagement::OEAssetType::Texture, CreateScope<AssetManagement::OETextureLoader>());

		m_ScriptingContext.InputSystem = m_InputSystem.get();
		m_ScriptingContext.Registry = &m_Scene->GetRegistry();
		m_ScriptingContext.AssetManager = m_AssetManager.get();

		OrionRenderer::ORDefaultRendererResources::Initialize();

		m_Renderer->InitShadowMap(2048); 

		m_Scene->InitScene();
		m_SceneRenderer = CreateScope<OrionRenderer::ORSceneRenderer>(m_Renderer.get(), m_AssetManager.get());
		m_ViewportFramebuffer = OrionRenderer::ORFrameBuffer::CreateFrameBuffer(1280, 720, OrionRenderer::ORRenderer::GetGraphicsAPI());

		if (!m_SceneRenderer->RenderSceneEnvironment(m_Scene->GetEnvironment()))
			ORION_ENGINE_WARNING("IBL Generation failed!");

		ORION_ENGINE_INFO("Engine subsystems initialized!");
	}

	void OEngine::OShutdownEngineSubsystems()
	{
		m_SceneRenderer.reset();
		OrionRenderer::ORDefaultRendererResources::Shutdown();
		m_Renderer->Shutdown();
	}

	void OEngine::OEngineShutdown()
	{
		OShutdownEngineSubsystems();
		ORION_ENGINE_INFO("Engine has shutdown!"); // just log or something
	}

	void OEngine::OTick(float dt)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");
		OE_CORE_ASSERT(m_SceneRenderer, "Scene renderer is null!");

		OEEnvironment& environment = m_Scene->GetEnvironment();

		if (environment.IsIBLDirty())
		{
			if (!m_SceneRenderer->RenderSceneEnvironment(environment))
				ORION_ENGINE_WARNING("IBL regeneration failed!");
		}

		// Update other systems here later
	}
	
	void OEngine::OBeginFrame()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		// TODO: Implement ORRenderer:BeginFrame and call it here
	}

	void OEngine::OEndFrame()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		// TODO: Implement ORRenderer::EndFrame and call it here
	}

	void OEngine::Render()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		OE_CORE_ASSERT(m_SceneRenderer, "Scene renderer is null!");
		OE_CORE_ASSERT(m_ViewportFramebuffer, "Viewport framebuffer is null!");

		OrionRenderer::ORRenderCommand::SetClearColor(0.2f, 0.3f, 0.4f, 1.0F);

		OrionRenderer::ORRenderCommand::Clear();

		m_SceneRenderer->RenderScene(m_Scene->GetRegistry(), m_Scene->GetCurrentCamera(), m_Scene->GetEnvironment(), m_Scene->GetDirectionalLight(), m_Scene->GetCurrentCamera().GetViewProjection());
	}
}
