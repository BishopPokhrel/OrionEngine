#pragma once

#include "ORCamera.h"

// ECS includes
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <OrionEngine/OrionRenderer/ORRenderer.h>
#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORSceneRenderer
		{
		public:

			explicit ORSceneRenderer(ORRenderer* renderer) 
				: m_Renderer(renderer)
			{
				OE_CORE_ASSERT(m_Renderer, "ORSceneRenderer received a null renderer (ORRenderer)!");
			}

			void RenderScene(ECS::OEECSRegistry& registry, const ORCamera& camera, const glm::mat4& viewProjection);
			void Render(ECS::OEECSRegistry& registry, ECS::OEGameEntityID entity);

		private:

			ORRenderer* m_Renderer = nullptr;
		};
	}
}