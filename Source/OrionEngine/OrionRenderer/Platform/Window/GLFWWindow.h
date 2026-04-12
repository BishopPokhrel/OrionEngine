#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <string>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		struct WindowProps
		{
			std::string Title;
			unsigned int Width;
			unsigned int Height;
			bool VSyncEnabled;
			
			WindowProps(const std::string& title = "Orion Engine", unsigned int width = 1280, unsigned int height = 800, bool vSyncEnabled = true)
				: Title(title), Width(width), Height(height), VSyncEnabled(vSyncEnabled)
			{}
		};
		
		class GLFWWindow
		{
		public:

			GLFWWindow(const WindowProps& props);
			~GLFWWindow();

			void OnUpdate();
			bool ShouldClose() const { return (glfwWindowShouldClose(m_Window) ? true : false); }
			int GetWidth() const { return m_Width; }
			int GetHeight() const { return m_Height; }
			void SetVSync(bool enabled);
			GLFWwindow* GetNativeWindow() { return m_Window; }	

			void Terminate() { glfwTerminate(); }

		private:

			GLFWwindow* m_Window = nullptr;
			int m_Width;
			int m_Height;
			std::string m_WindowTitle;
			bool m_VSyncEnabled;
		};
	}
}