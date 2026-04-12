#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OrionEngine/OrionRenderer/Platform/Window/GLFWWindow.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace OrionEditor
	{
		class OEMainWindow
		{
		public:

			bool InitOEMainWindow();
			bool MainLoop();
			bool DestoryOEMainWindow();

		private:

			OrionRenderer::GLFWWindow* m_Window;	
		};
	}
}