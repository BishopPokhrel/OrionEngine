#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <cstdint>

#include "OEGameEntity.h"

namespace OrionEngine
{
	namespace ECS
	{
		struct OETransformComponent
		{
			glm::vec3 position;
			glm::vec3 rotation;
			glm::vec3 scale;
		};

		struct OEPhysicsComponent
		{
			float mass = 0.0f;
			glm::vec3 velocity{ 0.0f };
			glm::vec3 accleration{ 0.0f };
			bool isStatic = true; // default
			float linearDrag = 0.0f;
			float radius = 0.0f;
		};

		struct OERenderableComponent
		{
			bool b_Visible = false;
			/*
				Later, add:
				Ref<Mesh> Mesh;
				Ref<Material> Material;
			*/
		};

		template<typename OEComponentType>
		class OEComponentManager
		{
		public:

			OEComponentType AddComponent(OEGameEntityID gameEntityID,  const OEComponentType& type) noexcept;
			void DeleteComponent(uint64_t id) noexcept;
			OEComponentType* GetComponent(uint64_t id) noexcept;
			const OEComponentType* GetComponent(uint64_t id) const noexcept;
			const std::unordered_map<uint64_t, OEComponentType>& GetAllComponents() const noexcept { return m_OEComponents; }
			std::unordered_map<uint64_t, OEComponentType>& GetAllComponents() noexcept { return m_OEComponents; }

		private:

			std::unordered_map<uint64_t, OEComponentType> m_OEComponents;
			uint64_t m_NextOEComponentID;
		};
	}
} 