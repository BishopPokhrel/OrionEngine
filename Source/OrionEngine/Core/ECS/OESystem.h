#pragma once

#include <OrionEngine/Core/ECS/OEECSRegistry.h>

namespace OrionEngine
{
	namespace ECS
	{
		class OESystem
		{
		public:

			virtual ~OESystem() = default;
			virtual void Update(OEECSRegistry& registry, float dt) noexcept = 0;
		};

		class OETransformSystem : public OESystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};

		class OEPhysicsSystem : public OESystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};

		class OERenderSystem : public OESystem
		{
		public:

			void Update(OEECSRegistry& registry, float dt) noexcept override;
		};
	}
}