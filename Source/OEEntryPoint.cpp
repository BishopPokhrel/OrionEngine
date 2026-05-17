#include <OrionEngine/Core/OEApplication.h>
#include <OrionEngine/Core/OEEngineDefines.h>
#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEExceptions.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>

#include <stdexcept>

using namespace OrionEngine;

#if defined(_WIN64)

static int OEngineMain(int argc, char** argv)
{
	ORION_ENGINE_INFO("Orion Engine has started!");

	auto app = OEApplication::CreateApplication();
	
	try
	{
		constexpr int width = 800;
		constexpr int height = 600;
		constexpr const char* title = "Orion Engine";

		app->EngineInit();
		app->EngineRun(width, height, title);
		app->EngineShutdown();

		ORION_ENGINE_INFO("All checks passed successfully!");
		return ENGINE_SUCCESS;
	}
	catch (const OEBaseException& e)
	{
		OE_CORE_ASSERT(false, "Engine Exception occurred!");
		if (app)
			app->EngineShutdown();
		
		ORION_ENGINE_ERROR(e.what());
		return ENGINE_FAILURE;
	}
	catch (const std::exception& e)
	{
		OE_CORE_ASSERT(false, "Standard Expection occurred!");
		if (app)
			app->EngineShutdown();
		ORION_ENGINE_ERROR(e.what());
		return ENGINE_FAILURE;
	}
	catch (...)
	{
		OE_CORE_ASSERT(false, "Unknown exception occurred!");
		if (app)
			app->EngineShutdown();

		ORION_ENGINE_ERROR("An unknown exception occurred!");
		return ENGINE_FAILURE;
	}
}

int main(int argc, char** argv)
{
	return OEngineMain(argc, argv);
}

#else

#if defined(__APPLE__)
#error "Orion Engine does not build on MacOS!"
#endif

#if defined(__linux__)
#error "Orion Engine does not build on Linux!"
#endif

#error "Unknown platform. Not supported."

#endif // defined(_WIN64)
