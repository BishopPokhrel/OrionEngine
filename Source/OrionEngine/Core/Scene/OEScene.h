#pragma once

#include <string>
#include <vector>

#include <OrionEngine/Core/ECS/OEGameEntity.h>

namespace OrionEngine
{
	class OEScene
	{
	public:

		explicit OEScene(const std::string& name) noexcept;
		// Entity functions
		ECS::OEGameEntityID CreateGameEntity(const std::string& name) noexcept;
		void DeleteGameEntity(ECS::OEGameEntityID id) noexcept;
		ECS::OEGameEntityID GetEntityIDByName(const std::string& name) const noexcept;
		std::string GetEntityNameByID(ECS::OEGameEntityID id) const noexcept;

	private:
		std::string m_OESceneName;
	
		struct OEGameEntityData
		{
			std::string Name;
			ECS::OEGameEntityID ID;
		};

		ECS::OEGameEntityID m_NextID = 0;
		std::vector<OEGameEntityData> m_OEGameEntities;
	};
}