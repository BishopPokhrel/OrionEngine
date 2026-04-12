#define GLFW_INCLUDE_NONE // Just in case

#include "OEMainWindow.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/OrionRenderer/ORRenderCommand.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLRendererAPI.h>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace OrionEngine::OrionRenderer
{
	OERRendererAPI* ORRenderCommand::s_RendererAPI = nullptr;
}

namespace OrionEngine
{
	namespace OrionEditor
	{
		using namespace OrionRenderer;
		bool OEMainWindow::InitOEMainWindow()
		{
			ORRenderCommand::s_RendererAPI = new OpenGLRendererAPI();
			ORRenderCommand::ORInit();

			m_Window = new GLFWWindow(WindowProps());
			return true;
		}
		
		bool OEMainWindow::MainLoop()
		{
			while (!m_Window->ShouldClose())
			{
				ORRenderCommand::Clear();
				m_Window->OnUpdate();
			}
			return true;
		}

		bool OEMainWindow::DestoryOEMainWindow()
		{
			delete m_Window;
			m_Window = nullptr;
			delete ORRenderCommand::s_RendererAPI;
			ORRenderCommand::s_RendererAPI = nullptr;
			return true;
		}
	}
}