#include "OEApplication.h"
#include "OELogging.h"

namespace OrionEngine
{
	bool OEApplication::EngineInit()
	{
		ORION_ENGINE_INFO("Orion Engine is initializing!");
		return true;
	}

	bool OEApplication::EngineRun(int width, int height, const char* title)
	{
		ORION_ENGINE_INFO("Orion Engine is running!");
		m_OEMainEngineWindow.InitOEMainWindow();
		m_OEMainEngineWindow.MainLoop();
		m_OEMainEngineWindow.DestoryOEMainWindow();
		return true;
	}

	bool OEApplication::EngineShutdown()
	{
		ORION_ENGINE_INFO("Orion Engine is shutting down!");
		return true;
	}

	Scope<OEApplication> OEApplication::CreateApplication()
	{
		return CreateScope<OEApplication>();
	}
}
