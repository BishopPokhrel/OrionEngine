#include "OEGameEntity.h"

namespace OrionEngine
{
	namespace ECS
	{
		OEGameEntityID OEGameEntityManager::CreateOEGameEntity(const std::string& name) noexcept
		{
			OEGameEntityData gameEntity;
			gameEntity.name = name;
			gameEntity.id = m_NextID++;
			m_OEGameEntities.push_back(gameEntity);
			return gameEntity.id;
		}

		bool OEGameEntityManager::DeleteOEGameEntity(OEGameEntityID id) noexcept
		{
			for (auto it = m_OEGameEntities.begin(); it != m_OEGameEntities.end(); it++)
			{
				if (it->id == id)
				{
					m_OEGameEntities.erase(it);
					return true;
				}
			}
			return false; // did not find entity
		}

		OEGameEntityID OEGameEntityManager::GetOEGameEntityIDByName(const std::string& name) noexcept
		{
			for (auto it = m_OEGameEntities.begin(); it != m_OEGameEntities.end(); it++)
			{
				if (it->name == name)
				{
					return it->id;
				}
			}
			return 0; // did not find entity
		}

		std::string OEGameEntityManager::GetOEGameEntityNameByID(OEGameEntityID id) noexcept
		{
			for (auto it = m_OEGameEntities.begin(); it != m_OEGameEntities.end(); it++)
			{
				if (it->id == id)
				{
					return it->name;
				}
			}
			return ""; // did not find entity
		}
	}
}