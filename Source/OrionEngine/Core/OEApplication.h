#pragma once

#define GLFW_INCLUDE_NONE
#include <OrionEditor/OEMainWindow.h>

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

		OrionEditor::OEMainWindow m_OEMainEngineWindow;
	};
}