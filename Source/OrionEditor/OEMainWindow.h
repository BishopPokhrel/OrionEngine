#pragma once
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OrionEngine/OrionRenderer/Platform/Window/GLFWWindow.h>
#include <OrionEngine/Core/OERef.h>

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/OrionRenderer/ORRenderCommand.h>
#include <OrionEngine/OrionRenderer/ORRenderer.h>
#include <OrionEngine/OrionRenderer/Platform/OpenGL/OpenGLRendererAPI.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/OrionRenderer/ORShader.h>

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
			Ref<OrionRenderer::ORVertexArray> m_VertexArray;
			Ref<OrionRenderer::ORShader> m_Shader;
		};
	}
}