#include <OrionEngine/Core/OEApplication.h>
#include <OrionEngine/Core/OEEngineDefines.h>
#include <OrionEngine/Core/OELogging.h>

#include <stdexcept>
#include <memory>

using namespace OrionEngine;

#ifdef _WIN64

int main()
{
	try
	{
		std::unique_ptr<Application> app(Application::CreateApplication());
		if (app == nullptr)
		{
			ORION_ENGINE_FATAL_ERROR("app variable is a nullptr!");
			return ENGINE_FAILURE;
		}

		app->EngineInit();
		app->EngineRun(800, 600, "Orion Engine");
		app->EngineShutdown();
		return ENGINE_SUCCESS;
	}
	catch (const std::exception& e)
	{
		ORION_ENGINE_ERROR(e.what());
		return ENGINE_FAILURE;
	}
	catch (...)
	{
		ORION_ENGINE_ERROR("Some unknown exception occurred!");
		return ENGINE_FAILURE;
	}
}

#else
#error "Orion Engine only builds on Windows 64 bit!"
#endif // _WIN64
