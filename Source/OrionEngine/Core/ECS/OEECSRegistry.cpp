#include "OEECSRegistry.h"
#include "OESystem.h"

namespace OrionEngine
{
    namespace ECS
    {
        OEGameEntityID OEECSRegistry::CreateGameEntity(const std::string& name) 
        {
            OEGameEntityData newGameEntity;
            newGameEntity.Name = name;
            newGameEntity.ID = m_NextID++;

            m_Entities.push_back(newGameEntity);

            // keep maps in sync
            m_NameToID[name] = newGameEntity.ID;
            m_IDToName[newGameEntity.ID] = name;

            return newGameEntity.ID;
        }

        void OEECSRegistry::DeleteGameEntity(ECS::OEGameEntityID id) 
        {
            for (auto it = m_Entities.begin(); it != m_Entities.end(); ++it)
            {
                if (it->ID == id)
                {
                    // remove from lookup maps
                    m_NameToID.erase(it->Name);
                    m_IDToName.erase(it->ID);

                    // remove components
                    TransformComponent.DeleteComponent(id);
                    PhysicsComponent.DeleteComponent(id);
                    RenderableComponent.DeleteComponent(id);
                    CameraComponent.DeleteComponent(id);

                    // remove entity
                    m_Entities.erase(it);
                    return;
                }
            }
        }

        OEGameEntityID OEECSRegistry::CopyGameEntity(OEGameEntityID originalEntityID)
        {
            if (!IsValid(originalEntityID) || originalEntityID == INVALID_GAME_ENTITY_ID)
                return INVALID_GAME_ENTITY_ID;

            std::string originalEntityName = GetEntityNameByID(originalEntityID);
            std::string newEntityName = originalEntityName + "Copy";

            if (GetEntityIDByName(newEntityName) != INVALID_GAME_ENTITY_ID)
                newEntityName = originalEntityName + "Copy" + std::to_string(m_CopiedNextNumber);

            OEGameEntityID newEntityID = CreateGameEntity(newEntityName);

            if (newEntityID == INVALID_GAME_ENTITY_ID)
                return INVALID_GAME_ENTITY_ID;

            if (HasComponent<OETransformComponent>(originalEntityID))
            {
                OETransformComponent* transform = GetComponent<OETransformComponent>(originalEntityID); // give the original entity's id so as to get their component and later copy it into the new entity
                TransformComponent.AddComponent(newEntityID, transform ? *transform : OETransformComponent()); // Deference the component pointer only if its not null otherwise provide the default transform component
            }

            if (HasComponent<OEPhysicsComponent>(originalEntityID))
            {
                OEPhysicsComponent* physics = GetComponent<OEPhysicsComponent>(originalEntityID);
                PhysicsComponent.AddComponent(newEntityID, physics ? *physics : OEPhysicsComponent());
            }

            if (HasComponent<OERenderableComponent>(originalEntityID))
            {
                OERenderableComponent* renderable = GetComponent<OERenderableComponent>(originalEntityID);
                RenderableComponent.AddComponent(newEntityID, renderable ? *renderable : OERenderableComponent());
            }

            if (HasComponent<OECameraComponent>(originalEntityID))
            {
                OECameraComponent* camera = GetComponent<OECameraComponent>(originalEntityID);
                CameraComponent.AddComponent(newEntityID, camera ? *camera : OECameraComponent());
            }

            m_CopiedNextNumber++;
            return newEntityID;
        }

        ECS::OEGameEntityID OEECSRegistry::GetEntityIDByName(const std::string& name) const noexcept
        {
            auto it = m_NameToID.find(name);
            if (it != m_NameToID.end())
                return it->second;

            return 0; // invalid ID
        }

        std::string OEECSRegistry::GetEntityNameByID(ECS::OEGameEntityID id) const noexcept
        {
            auto it = m_IDToName.find(id);
            if (it != m_IDToName.end())
                return it->second;

            return ""; // not found
        }

        void OEECSRegistry::SetEntityName(OEGameEntityID id, const std::string& name)
        {
            if (!IsValid(id))
                return;

            // remove old mapping safely
            auto it = m_IDToName.find(id);
            if (it != m_IDToName.end())
            {
                m_NameToID.erase(it->second);
            }

            // update maps
            m_IDToName[id] = name;
            m_NameToID[name] = id;

            // update entity storage
            for (auto& e : m_Entities)
            {
                if (e.ID == id)
                {
                    e.Name = name;
                    break;
                }
            }
        }

        std::vector<OEGameEntityID> OEECSRegistry::GetAllEntityIDs() const noexcept
        {
            std::vector<OEGameEntityID> ids;
            ids.reserve(m_Entities.size());

            for (const auto& e : m_Entities)
                ids.push_back(e.ID);

            return ids;
        }

        // This function is to simply delete ALL the entities in a scene. 
        // Since everything is in the registry, we delete everything in that current registry. 
        // This function is only meanted to be used when the scene is to be deleted, NOT when the scene is to be loaded or unloaded.
       
        void OEECSRegistry::DeleteRegistry() noexcept
        {
            // CLEANUP ENTITIES
            m_Entities.clear(); 
            m_NameToID.clear();
            m_IDToName.clear();

            // CLEANUP COMPONENTS
            TransformComponent.Clear();
            PhysicsComponent.Clear();
            RenderableComponent.Clear();
            CameraComponent.Clear();

            m_NextID = 1;
        }
    }
}