#include "OESystem.h"

namespace OrionEngine
{
	namespace ECS
	{
		void OETransformSystem::Update(OEECSRegistry& registry, float dt) noexcept
		{
			auto& transforms = registry.TranformComponent.GetAllComponents();
			
			for (auto& [entity, transform] : transforms)
			{
				transform.position.x += 1.0f * dt;
				transform.position.y += 1.0f * dt;
				transform.position.z += 1.0f * dt;

				transform.rotation.x += 1.0f * dt;
				transform.rotation.y += 1.0f * dt;
				transform.rotation.z += 1.0f * dt;

				transform.scale.x += 1.0f * dt;
				transform.scale.y += 1.0f * dt;
				transform.scale.z += 1.0f * dt;
			}
		}

		void OEPhysicsSystem::Update(OEECSRegistry& registry, float dt) noexcept
		{
			auto& physicsMap = registry.PhysicsComponent.GetAllComponents();

			for (auto& [entity, physics] : physicsMap)
			{
				auto* transform = registry.TranformComponent.GetComponent(entity); // entity will be uint64_t id
				if (!transform)
					continue;

				if (physics.isStatic)
					continue;

				physics.velocity.x += physics.accleration.x * dt;
				physics.velocity.y += physics.accleration.y * dt;
				physics.velocity.z += physics.accleration.z * dt;

				physics.velocity.x *= (1.0f - physics.linearDrag * dt);
				physics.velocity.y *= (1.0f - physics.linearDrag * dt);
				physics.velocity.z *= (1.0f - physics.linearDrag * dt);

				transform->position.x += physics.velocity.x * dt;	
				transform->position.y += physics.velocity.y * dt;	
				transform->position.z += physics.velocity.z * dt;	
			}
		}

		void OERenderSystem::Update(OEECSRegistry& registry, float dt) noexcept
		{
			auto& renderable = registry.RenderableComponent.GetAllComponents();

			for (auto& [entity, render] : renderable)
			{
				render.b_Visible = true;
			}
		}
	}
}