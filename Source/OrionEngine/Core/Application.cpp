#include "Application.h"
#include "Logging.h"

namespace OrionEngine
{
	bool Application::EngineInit()
	{
		ORION_ENGINE_INFO("Orion Engine is initializing!");
		return true;
	}

	bool Application::EngineRun()
	{
		ORION_ENGINE_INFO("Orion Engine is running!");
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
