#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace OrionEngine
{
	namespace ECS
	{
		using OEGameEntityID = uint64_t;
		struct OEGameEntityData
		{
			std::string name;
			OEGameEntityID id;
		};

		class OEGameEntityManager
		{
		public:

			OEGameEntityID CreateOEGameEntity(const std::string& gameEntityName) noexcept;
			bool DeleteOEGameEntity(OEGameEntityID id) noexcept;
			OEGameEntityID GetOEGameEntityIDByName(const std::string& gameEntityName) noexcept;
			std::string GetOEGameEntityNameByID(OEGameEntityID id) noexcept;

		private:

			std::vector<OEGameEntityData> m_OEGameEntities;
			OEGameEntityID m_NextID;
		};
	}
}