#include "OEngine.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	void OEngine::OInitEngineSubsystems()
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");
		m_Scene->InitScene();
		m_Renderer->Init(OrionRenderer::ORGraphicsAPI::OpenGL);
		OrionRenderer::ORDefaultRendererResources::Initialize();
		m_SceneRenderer = CreateScope<OrionRenderer::ORSceneRenderer>(m_Renderer.get()); 
		m_ViewportFramebuffer = OrionRenderer::ORFrameBuffer::CreateFrameBuffer(1280, 720, m_Renderer->GetGraphicsAPI());
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

		m_ViewportFramebuffer->Bind();

		OrionRenderer::ORRenderCommand::SetClearColor(
			0.2f,
			0.3f,
			0.4f,
			1.0f
		);

		OrionRenderer::ORRenderCommand::Clear();

		m_SceneRenderer->RenderScene(
			m_Scene->GetRegistry(),
			m_Scene->GetCurrentCamera(),
			m_Scene->GetCurrentCamera().GetViewProjection()
		);

		m_ViewportFramebuffer->Unbind();
	}
}
