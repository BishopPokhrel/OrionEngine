#pragma once

#define GLFW_INCLUDE_NONE
#include <OrionEditor/OEMainWindow.h>

namespace OrionEngine
{
	class Application
	{
	public:
		Application() = default;
		~Application() = default;

		bool EngineInit();
		bool EngineRun(int width, int height, const char* title);
		bool EngineShutdown();

		static Application* CreateApplication();

	private:

		OrionEditor::OEMainWindow m_OEMainEngineWindow;
	};
}