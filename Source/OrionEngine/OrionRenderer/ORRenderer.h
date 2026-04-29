#pragma once

#include <glm/glm.hpp>
#include "ORGraphicsAPI.h"

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderer
		{
		public:

			static void Init()
			{

			}

			static void Shutdown()
			{

			}

			static void BeginScene(const glm::mat4& viewProjection) noexcept
			{
				
			}

			static void EndScene() noexcept
			{

			}

			static void Submit() noexcept
			{

			}

			static void SetGraphicsAPI(ORGraphicsAPI API) noexcept
			{
				s_GraphicsAPI = API;
			}

			static ORGraphicsAPI GetGraphicsAPI() noexcept { return s_GraphicsAPI; }

		private:

			static ORGraphicsAPI s_GraphicsAPI;
		};
	}
}