#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

namespace OrionEngine
{
	namespace OrionEditor
	{
		class OEMainWindow
		{
		public:

			bool InitOEMainWindow();
			bool CreateOEMainWindow(int width, int height, const char* title);
			bool MainLoop();
			bool DestoryOEMainWindow();

		private:

			GLFWwindow* m_OEMainWindow = nullptr;
		};
	}
}