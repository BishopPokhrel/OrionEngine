#pragma once

#include "ORCamera.h"

// ECS includes
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORSceneRenderer
		{
		public:

			void RenderScene(ECS::OEECSRegistry& registry, const ORCamera& camera, const glm::mat4& viewProjection);
			void Render(ECS::OEECSRegistry& registry, ECS::OEGameEntityID entity);
		};
	}
}