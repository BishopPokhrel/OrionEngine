#pragma once

#include "OEGameEntity.h"
#include "OEComponent.h"

#include <vector>
#include <string>
#include <unordered_map>
#include <optional>

namespace OrionEngine
{
    namespace ECS
    {
        class OEECSRegistry
        {
        private:

            struct OEGameEntityData
            {
                std::string Name;
                OEGameEntityID ID;
            };

        public:

            OEGameEntityID CreateGameEntity(const std::string& name) noexcept;
            void DeleteGameEntity(OEGameEntityID id) noexcept;

            bool IsValid(OEGameEntityID id) const noexcept
            {
                return m_IDToName.find(id) != m_IDToName.end();
            }

            OEGameEntityID GetEntityIDByName(const std::string& name) const noexcept;
            std::string GetEntityNameByID(OEGameEntityID id) const noexcept;

            void SetEntityName(OEGameEntityID id, const std::string& name);

            const std::vector<OEGameEntityID>& GetAllEntityIDs() const noexcept;
            const std::vector<OEGameEntityData>& GetAllGameEntities() const noexcept { return m_Entities; }
            void ResizeVectorToMax() {
                if (m_Entities.size() >= ECS::MAX_GAME_ENTITIES)
                    m_Entities.resize(ECS::MAX_GAME_ENTITIES);
                return;
            }

            template<typename T>
            T* GetComponent(OEGameEntityID id) 
            {
                static_assert(sizeof(T) == 0, "GetComponent<T> not specialized for this type");
                return nullptr;
            }

            template<typename T>
            const T* GetComponent(OEGameEntityID id) const 
            {
                static_assert(sizeof(T) == 0, "GetComponent<T> not specialized for this type");
                return nullptr;
            }
            template<typename T>
            bool HasComponent(OEGameEntityID id) const { return false;  }

            OEComponentManager<OETransformComponent> TransformComponent;
            OEComponentManager<OEPhysicsComponent> PhysicsComponent;
            OEComponentManager<OERenderableComponent> RenderableComponent;
            OEComponentManager<OECameraComponent> CameraComponent;

        private:

            OEGameEntityID m_NextID = 0;

            std::vector<OEGameEntityData> m_Entities;

            std::unordered_map<std::string, OEGameEntityID> m_NameToID;
            std::unordered_map<OEGameEntityID, std::string> m_IDToName;

        };

        template<>
        inline OETransformComponent* OEECSRegistry::GetComponent<OETransformComponent>(OEGameEntityID id) 
        {
            return TransformComponent.GetComponent(id);
        }

        template<>
        inline OEPhysicsComponent* OEECSRegistry::GetComponent<OEPhysicsComponent>(OEGameEntityID id)
        {
            return PhysicsComponent.GetComponent(id);
        }

        template<>
        inline OERenderableComponent* OEECSRegistry::GetComponent<OERenderableComponent>(OEGameEntityID id)
        {
            return RenderableComponent.GetComponent(id);
        }

        template<>
        inline OECameraComponent* OEECSRegistry::GetComponent<OECameraComponent>(OEGameEntityID id)
        {
            return CameraComponent.GetComponent(id);
        }

        template<>
        inline bool OEECSRegistry::HasComponent<OETransformComponent>(OEGameEntityID id) const
        {
            return TransformComponent.HasComponent(id);
        }

        template<>
        inline bool OEECSRegistry::HasComponent<OERenderableComponent>(OEGameEntityID id) const
        {
            return RenderableComponent.HasComponent(id);
        }

        template<>
        inline bool OEECSRegistry::HasComponent<OEPhysicsComponent>(OEGameEntityID id) const
        {
            return PhysicsComponent.HasComponent(id);
        }

        template<>
        inline bool OEECSRegistry::HasComponent<OECameraComponent>(OEGameEntityID id) const
        {
            return CameraComponent.HasComponent(id);
        }
    }
}