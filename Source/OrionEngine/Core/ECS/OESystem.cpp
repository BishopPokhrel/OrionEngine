#include "OESystem.h"

namespace OrionEngine
{
	namespace ECS
	{
		void OETransformSystem::Update(OEECSRegistry& registry, float dt) noexcept
		{
			auto& transforms = registry.TransformComponent.GetAllComponents();
			
			for (auto& [entity, transform] : transforms)
			{
				transform.Position.x += 1.0f * dt;
				transform.Position.y += 1.0f * dt;
				transform.Position.z += 1.0f * dt;

				transform.Rotation.x += 1.0f * dt;
				transform.Rotation.y += 1.0f * dt;
				transform.Rotation.z += 1.0f * dt;

				transform.Scale.x += 1.0f * dt;
				transform.Scale.y += 1.0f * dt;
				transform.Scale.z += 1.0f * dt;
			}
		}

		void OEPhysicsSystem::Update(OEECSRegistry& registry, float dt) noexcept
		{
			auto& physicsMap = registry.PhysicsComponent.GetAllComponents();

			for (auto& [entity, physics] : physicsMap)
			{
				auto* transform = registry.TransformComponent.GetComponent(entity); // entity will be uint64_t id
				if (!transform)
					continue;

				if (physics.IsStatic)
					continue;

				physics.Velocity.x += physics.Accleration.x * dt;
				physics.Velocity.y += physics.Accleration.y * dt;
				physics.Velocity.z += physics.Accleration.z * dt;

				physics.Velocity.x *= (1.0f - physics.LinearDrag * dt);
				physics.Velocity.y *= (1.0f - physics.LinearDrag * dt);
				physics.Velocity.z *= (1.0f - physics.LinearDrag * dt);

				transform->Position.x += physics.Velocity.x * dt;	
				transform->Position.y += physics.Velocity.y * dt;	
				transform->Position.z += physics.Velocity.z * dt;	
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