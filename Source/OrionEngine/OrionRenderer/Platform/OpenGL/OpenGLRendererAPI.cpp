#define GLFW_INCLUDE_NONE
#include "OpenGLRendererAPI.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		void OpenGLRendererAPI::Init()
		{
		}

		void OpenGLRendererAPI::Clear()
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
		{
			glViewport(x, y, width, height);
		}

		void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
		}

		void OpenGLRendererAPI::DrawIndexed(uint32_t indexCount)
		{
			glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
		}
	}
}