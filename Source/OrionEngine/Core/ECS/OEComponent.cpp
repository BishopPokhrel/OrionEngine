#include "OEComponent.h"

namespace OrionEngine
{
	namespace ECS
	{
		template<typename OEComponentType>
		OEComponentType OEComponentManager<OEComponentType>::AddComponent(OEGameEntityID gameEntityID,  const OEComponentType& type) noexcept
		{
			m_OEComponents[gameEntityID] = type;
			return type;
		}

		template<typename OEComponentType>
		void OEComponentManager<OEComponentType>::DeleteComponent(uint64_t id) noexcept
		{
			auto it = m_OEComponents.find(id);
			if (it != m_OEComponents.end())
			{
				m_OEComponents.erase(it);
			}
		}

		template<typename OEComponentType>
		OEComponentType* OEComponentManager<OEComponentType>::GetComponentType(uint64_t id) noexcept
		{
			auto it = m_OEComponents.find(id);
			if (it != m_OEComponents.end())
				return &it->second;
			else
				return nullptr;
		}
	}
}