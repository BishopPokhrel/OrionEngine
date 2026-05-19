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
		return true;
	}

	bool OEApplication::EngineRun(int width, int height, const char* title)
	{
		ORION_ENGINE_INFO("Orion Engine is running!");

		m_OEMainEngineWindow.InitOEMainWindow();
		m_Running = true;

		while (m_Running && !m_OEMainEngineWindow.GetOEWindow()->ShouldClose())
		{
			float dt = OETime::OEGetDeltaTime();

			m_OEngine->OBeginFrame();

			m_OEngine->OTick(dt);
			m_OEngine->GetInputSystem()->Update(m_OEMainEngineWindow.GetOEWindow()->GetNativeWindow());
			m_OEngine->Render();

			m_OEMainEngineWindow.GetOEWindow()->OnUpdate();

			m_OEngine->OEndFrame();
		}

		m_OEMainEngineWindow.DestoryOEMainWindow();

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
