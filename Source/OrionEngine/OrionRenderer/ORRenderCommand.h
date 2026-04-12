#pragma once

#include "ORRendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderCommand
		{
		public:

			static OERRendererAPI* s_RendererAPI;

			static void ORInit()
			{
				if (!s_RendererAPI)
					s_RendererAPI = new OpenGLRendererAPI();

				s_RendererAPI->Init();
			}

			static void Clear()
			{
				s_RendererAPI->Clear();
			}

			static void SetViewport(int x, int y, int width, int height)
			{
				s_RendererAPI->SetViewport(x, y, width, height);
			}

			static void SetClearColor(float r, float g, float b, float a)
			{
				s_RendererAPI->SetClearColor(r, g, b, a);
			}

			static void DrawIndexed(uint32_t indexCount)
			{
				s_RendererAPI->DrawIndexed(indexCount);
			}
		};
	}
}