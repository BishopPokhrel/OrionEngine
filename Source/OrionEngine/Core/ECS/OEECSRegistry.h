#pragma once

#include "OEGameEntity.h"
#include "OEComponent.h"

namespace OrionEngine
{
	namespace ECS
	{
		class OEECSRegistry
		{
		public:

			OEGameEntityID CreateGameEntity(const std::string& name) noexcept;
			void DeleteGameEntity(OEGameEntityID id) noexcept;
			OEGameEntityID GetEntityIDByName(const std::string& name) const noexcept;
			std::string GetEntityNameByID(OEGameEntityID id) const noexcept;

			OEComponentManager<OETransformComponent> TranformComponent;
			OEComponentManager<OEPhysicsComponent> PhysicsComponent;
			OEComponentManager<OERenderableComponent> RenderableComponent;

		private:

			struct OEGameEntityData
			{
				std::string Name;
				ECS::OEGameEntityID ID;
			};

			ECS::OEGameEntityID m_NextID = 0;
			std::vector<OEGameEntityData> m_OEGameEntities;
		};
	}
}