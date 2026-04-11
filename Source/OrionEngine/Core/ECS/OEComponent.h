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
			float vx, vy, vz = 0.0f;
			float ax, ay, az = 0.0f;
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
			OEComponentType* GetComponentType(uint64_t id) noexcept;

		private:

			std::unordered_map<uint64_t, OEComponentType> m_OEComponents;
			uint64_t m_NextOEComponentID;
		};
	}
} 