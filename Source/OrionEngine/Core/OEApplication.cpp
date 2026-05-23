#include "OEApplication.h"
#include "OELogging.h"
#include "OETime.h"

namespace OrionEngine
{
	bool OEApplication::EngineInit()
	{
		ORION_ENGINE_INFO("Orion Engine is initializing!");
		m_OEngine = CreateScope<OEngine>();
		m_OEngine->OInitEngineSubsystems();
		
		// Intialize or create the window
		m_WindowProps.Width = 1280;
		m_WindowProps.Height = 800;
		m_WindowProps.Title = "Orion Engine";
		m_WindowProps.VSyncEnabled = true;
		m_Window = CreateScope<OrionRenderer::GLFWWindow>(m_WindowProps);
		return true;
	}

	bool OEApplication::EngineRun(int width, int height, const char* title)
	{
		ORION_ENGINE_INFO("Orion Engine is running!");

		m_Running = true;

		while (m_Running && !m_Window->ShouldClose())
		{
			float dt = OETime::OEGetDeltaTime();

			m_OEngine->OBeginFrame();

			m_OEngine->OTick(dt);
			m_OEngine->GetInputSystem()->Update(m_Window->GetNativeWindow());
			m_OEngine->Render();

			m_Window->OnUpdate();

			m_OEngine->OEndFrame();
		}

		return true;
	}

	bool OEApplication::EngineShutdown()
	{
		ORION_ENGINE_INFO("Orion Engine is shutting down!");
		
		m_OEngine->OShutdownEngineSubsystems(); // Shutdown the subsystems first
		m_OEngine->OEngineShutdown(); // Fully shut down the engine
		return true;
	}

	Scope<OEApplication> OEApplication::CreateApplication()
	{
		return CreateScope<OEApplication>();
	}
}
