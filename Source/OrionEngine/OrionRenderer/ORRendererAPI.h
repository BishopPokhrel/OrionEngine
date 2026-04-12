#pragma once

#include <cstdint>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class OERRendererAPI
		{
		public:

			virtual ~OERRendererAPI() = default;

			virtual void Init() = 0;
			virtual void Clear() = 0;
			virtual void SetViewport(int x, int y, int width, int height) = 0;
			virtual void SetClearColor(float r, float g, float b, float a) = 0;
			virtual void DrawIndexed(uint32_t indexCount) = 0;
		};
	}
}