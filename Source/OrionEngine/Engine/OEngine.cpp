#include "OEngine.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	void OEngine::OInitEngineSubsystems()
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");
		m_Scene->InitScene();
	}

	void OEngine::OShutdownEngineSubsystems()
	{
		// Unload/shutdown later (TODO)
	}

	void OEngine::OEngineShutdown()
	{
		OShutdownEngineSubsystems();
		ORION_ENGINE_INFO("Engine has shutdown!"); // just log or something
	}

	void OEngine::OTick(float dt)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");
		m_Scene->UpdateSystems(m_Scene->GetRegistry(), dt);
	}

	void OEngine::OBeginFrame()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		m_Renderer->BeginFrame();
	}

	void OEngine::OEndFrame()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		m_Renderer->EndFrame();
	}

	void OEngine::Render()
	{
		OE_CORE_ASSERT(m_Renderer, "Renderer is null!");
		m_Renderer->BeginScene(m_Scene->GetCurrentCamera()->GetViewProjection());
		m_SceneRenderer->RenderScene(m_Scene->GetRegistry(), m_Scene->GetCurrentCamera(), m_Scene->GetCurrentCamera()->GetViewProjection());
		m_Renderer->EndScene();
	}
}
