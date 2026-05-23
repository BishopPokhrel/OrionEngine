#pragma once

#define GLFW_INCLUDE_NONE

#include <OrionEngine/Engine/OEngine.h>
#include <OrionEngine/OrionRenderer/Platform/Window/GLFWWindow.h>

#include "OERef.h"

namespace OrionEngine
{
	class OEApplication
	{
	public:
		OEApplication() = default;
		~OEApplication() = default;

		bool EngineInit();
		bool EngineRun(int width, int height, const char* title);
		bool EngineShutdown();

		static Scope<OEApplication> CreateApplication();

	private:

		Scope<OEngine> m_OEngine;
		Scope<OrionRenderer::GLFWWindow> m_Window;
		OrionRenderer::WindowProps m_WindowProps;

		bool m_Running = false; 
	};
}