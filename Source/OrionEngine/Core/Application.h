#pragma once

namespace OrionEngine
{
	class Application
	{
	public:
		Application() = default;
		~Application() = default;

		bool EngineInit();
		bool EngineRun();
		bool EngineShutdown();

		static Application* CreateApplication();

	private:

	};
}