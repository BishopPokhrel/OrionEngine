#pragma once

#include <OrionEngine/Core/ECS/OEECSRegistry.h>

namespace OrionEngine
{
	namespace ECS
	{
		class OEBaseSystem
		{
		public:

			virtual ~OEBaseSystem() = default;
			virtual void Update(OEECSRegistry& registry, float dt) noexcept = 0;
		};

		class OETransformSystem : public OEBaseSystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};

		class OEPhysicsSystem : public OEBaseSystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};

		class OERenderSystem : public OEBaseSystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};
	}
}