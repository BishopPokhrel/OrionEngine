#define GLFW_INCLUDE_NONE // Just in case

#include "OEMainWindow.h"
#include <OrionEngine/Core/OELogging.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OrionEngine
{
	namespace OrionEditor
	{
		bool OEMainWindow::InitOEMainWindow()
		{
			if (!glfwInit())
			{
				ORION_ENGINE_ERROR("Failed to initialize GLFW.");
				glfwTerminate();
				return false;
			}

			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			return true;
		}

		bool OEMainWindow::CreateOEMainWindow(int width, int height, const char* title)
		{
			m_OEMainWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
			if (!m_OEMainWindow)
			{
				ORION_ENGINE_ERROR("Failed to create GLFW window.");
				glfwTerminate();
				return false;
			}

			glfwMakeContextCurrent(m_OEMainWindow);
			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			{
				ORION_ENGINE_ERROR("Failed to initialize GLAD.");
				glfwTerminate();
				return false;
			}

			glClearColor(0.08f, 0.10f, 0.18f, 1.0f);
			return true;
		}

		bool OEMainWindow::MainLoop()
		{
			if (!m_OEMainWindow) return false;
			while (!glfwWindowShouldClose(m_OEMainWindow))
			{
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glfwSwapBuffers(m_OEMainWindow);
				glfwPollEvents();
			}
			return true;
		}

		bool OEMainWindow::DestoryOEMainWindow()
		{
			if (!m_OEMainWindow) return false;
			glfwDestroyWindow(m_OEMainWindow);
			glfwTerminate();
			return true;
		}
	}
}