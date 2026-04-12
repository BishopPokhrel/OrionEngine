#pragma once

#include <OrionEngine/OrionRenderer/ORRendererAPI.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OpenGLRendererAPI : public OERRendererAPI
		{
		public:

			void Init() override;
			void Clear() override;
			void SetViewport(int x, int y, int width, int height) override;
			void SetClearColor(float r, float g, float b, float a) override;
			void DrawIndexed(uint32_t indexCount) override;
		};
	}
}