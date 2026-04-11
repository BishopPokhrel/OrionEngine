#include "OEScene.h"

namespace OrionEngine
{
	OEScene::OEScene(const std::string& name) noexcept
		: m_OESceneName(name)
	{
	}

	ECS::OEGameEntityID OEScene::CreateGameEntity(const std::string& name) noexcept
	{
		OEGameEntityData newGameEntity;
		newGameEntity.Name = name;
		newGameEntity.ID = m_NextID++;
		m_OEGameEntities.push_back(newGameEntity);
		return newGameEntity.ID;
	}

	void OEScene::DeleteGameEntity(ECS::OEGameEntityID id) noexcept
	{
		for (auto it = m_OEGameEntities.begin(); it != m_OEGameEntities.end(); it++)
		{
			if (it->ID == id)
			{
				m_OEGameEntities.erase(it);
				return;
			}
		}
		return; // did not find
	}

	ECS::OEGameEntityID OEScene::GetEntityIDByName(const std::string& name) const noexcept
	{
		for (auto& entity : m_OEGameEntities)
		{
			if (entity.Name == name)
				return entity.ID;
		}
		return 0; // did not find.
	}

	std::string OEScene::GetEntityNameByID(ECS::OEGameEntityID id) const noexcept
	{
		for (auto& entity : m_OEGameEntities)
		{
			if (entity.ID == id)
				return entity.Name;
		}
		return ""; // did not find
	}
}