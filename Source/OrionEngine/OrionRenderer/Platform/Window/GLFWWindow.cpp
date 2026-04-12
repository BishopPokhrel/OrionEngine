#define GLFW_INCLUDE_NONE
#include "GLFWWindow.h"
#include <glad/glad.h>

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		GLFWWindow::GLFWWindow(const WindowProps& props)
		{
			m_Width = props.Width;
			m_Height = props.Height;
			m_WindowTitle = props.Title;
			m_VSyncEnabled = props.VSyncEnabled;	

			if (!glfwInit())
			{
				ORION_ENGINE_FATAL_ERROR("Failed to initialize GLFW.");
				return;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			
			m_Window = glfwCreateWindow(m_Width, m_Height, m_WindowTitle.c_str(), nullptr, nullptr);
			if (!m_Window)
			{
				ORION_ENGINE_ERROR("Failed to create GLFW window.");
				return;
			}

			glfwMakeContextCurrent(m_Window);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				ORION_ENGINE_FATAL_ERROR("Failed to initialize GLAD.");
				return;
			}
			SetVSync(m_VSyncEnabled);
		}

		GLFWWindow::~GLFWWindow()
		{
			if (m_Window)
			{
				glfwDestroyWindow(m_Window);
				m_Window = nullptr;
			}
		}

		void GLFWWindow::OnUpdate()
		{
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
		}

		void GLFWWindow::SetVSync(bool enabled)
		{
			glfwSwapInterval(enabled ? 1 : 0);
		}
	}
}