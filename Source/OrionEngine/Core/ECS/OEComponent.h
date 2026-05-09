#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <cstdint>

#include "OEGameEntity.h"

#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/OrionRenderer/ORShader.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace ECS
	{
		struct OETransformComponent
		{
			glm::vec3 Position;
			glm::vec3 Rotation;
			glm::vec3 Scale;
		};

		struct OEPhysicsComponent
		{
			float Mass = 0.0f;
			glm::vec3 Velocity{ 0.0f };
			glm::vec3 Accleration{ 0.0f };
			bool IsStatic = true; // default
			float LinearDrag = 0.0f;
			float Radius = 0.0f;
		};

		struct OERenderableComponent
		{
			bool b_Visible = false;
			Ref<OrionRenderer::ORShader> Shader;
			Ref<OrionRenderer::ORVertexArray> VertexArray;
		};

		template<typename OEComponentType>
		class OEComponentManager
		{
		public:

			OEComponentType AddComponent(OEGameEntityID gameEntityID, const OEComponentType& type) noexcept
			{
				m_OEComponents[gameEntityID] = type;
				return type;
			}

			void DeleteComponent(uint64_t id) noexcept
			{
				auto it = m_OEComponents.find(id);
				if (it != m_OEComponents.end())
				{
					m_OEComponents.erase(it);
				}
			}
			OEComponentType* GetComponent(uint64_t id) noexcept
			{
				auto it = m_OEComponents.find(id);
				if (it != m_OEComponents.end())
					return &it->second;
				else
					return nullptr;
			}

			bool HasComponent(uint64_t id) const noexcept {
				return m_OEComponents.find(id) != m_OEComponents.end();
			}

			const std::unordered_map<uint64_t, OEComponentType>& GetAllComponents() const noexcept { return m_OEComponents; }
			std::unordered_map<uint64_t, OEComponentType>& GetAllComponents() noexcept { return m_OEComponents; }

		private:

			std::unordered_map<uint64_t, OEComponentType> m_OEComponents;
			uint64_t m_NextOEComponentID;
		};
	}
} 