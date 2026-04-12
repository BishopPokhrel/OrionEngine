#include "OEApplication.h"
#include "OELogging.h"

namespace OrionEngine
{
	bool Application::EngineInit()
	{
		ORION_ENGINE_INFO("Orion Engine is initializing!");
		return true;
	}

	bool Application::EngineRun(int width, int height, const char* title)
	{
		ORION_ENGINE_INFO("Orion Engine is running!");
		m_OEMainEngineWindow.InitOEMainWindow();
		m_OEMainEngineWindow.MainLoop();
		m_OEMainEngineWindow.DestoryOEMainWindow();
		return true;
	}

	bool Application::EngineShutdown()
	{
		ORION_ENGINE_INFO("Orion Engine is shutting down!");
		return true;
	}

	Application* Application::CreateApplication()
	{
		return new Application();
	}
}
