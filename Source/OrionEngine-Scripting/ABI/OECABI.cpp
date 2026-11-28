#include "OECABI.h"

#include "../OEScriptingInternal.h"

#include <OrionEngine/Core/OEScriptingContext.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/AssetManagement/OEAssetManager.h>
#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMaterial.h>
#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMesh.h>
#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OETexture.h>
#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OETime.h>
#include <OrionEngine/Core/OEInputSystem.h>

#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>
#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>

namespace
{
	OrionEngine::OEScriptingContext* s_ScriptingContext = nullptr;
}

void OrionEngine::OEInitializeScriptingContext(OrionEngine::OEScriptingContext* context)
{
	s_ScriptingContext = context;
}
namespace ScriptingUtils
{
	std::string GetStringKeyCode(OEKeyCodes key)
	{
		switch (key)
		{
			case OE_KEY_UNKNOWN: return "UNKNOWN";

			case OE_KEY_SPACE: return "SPACE";
			case OE_KEY_ESCAPE: return "ESC";
			case OE_KEY_ENTER: return "ENTER";
			case OE_KEY_TAB: return "TAB";
			case OE_KEY_SHIFT: return "SHIFT";
			case OE_KEY_CTRL: return "CTRL";
			case OE_KEY_ALT: return "ALT";

			case OE_KEY_UP: return "UP";
			case OE_KEY_DOWN: return "DOWN";
			case OE_KEY_LEFT: return "LEFT";
			case OE_KEY_RIGHT: return "RIGHT";

			case OE_KEY_0: return "0";
			case OE_KEY_1: return "1";
			case OE_KEY_2: return "2";
			case OE_KEY_3: return "3";
			case OE_KEY_4: return "4";
			case OE_KEY_5: return "5";
			case OE_KEY_6: return "6";
			case OE_KEY_7: return "7";
			case OE_KEY_8: return "8";
			case OE_KEY_9: return "9";

			case OE_KEY_A: return "A";
			case OE_KEY_B: return "B";
			case OE_KEY_C: return "C";
			case OE_KEY_D: return "D";
			case OE_KEY_E: return "E";
			case OE_KEY_F: return "F";
			case OE_KEY_G: return "G";
			case OE_KEY_H: return "H";
			case OE_KEY_I: return "I";
			case OE_KEY_J: return "J";
			case OE_KEY_K: return "K";
			case OE_KEY_L: return "L";
			case OE_KEY_M: return "M";
			case OE_KEY_N: return "N";
			case OE_KEY_O: return "O";
			case OE_KEY_P: return "P";
			case OE_KEY_Q: return "Q";
			case OE_KEY_R: return "R";
			case OE_KEY_S: return "S";
			case OE_KEY_T: return "T";
			case OE_KEY_U: return "U";
			case OE_KEY_V: return "V";
			case OE_KEY_W: return "W";
			case OE_KEY_X: return "X";
			case OE_KEY_Y: return "Y";
			case OE_KEY_Z: return "Z";

			default: return "UNKNOWN";
		}
	}
}

extern "C"
{
	OE_API void LogMessage(OELogLevel level, const char* message)
	{
		switch (level)
		{
			case OELogLevel::DEBUG:
				ORION_ENGINE_DEBUG(message);
				break;
			case OELogLevel::INFO:
				ORION_ENGINE_INFO(message);
				break;
			case OELogLevel::WARNING:
				ORION_ENGINE_WARNING(message);
				break;
			case OELogLevel::ERROR:
				ORION_ENGINE_ERROR(message);
				break;
			case OELogLevel::FATAL:
				ORION_ENGINE_FATAL_ERROR(message);
				break;

			default: return;
		}
	}

	OE_API OEString GetEngineVersionNumber()
	{
		return "Orion Engine Version: v0.1";
	}

	OE_API OEUint32_t GetABIVersion(void)
	{
		return OE_ABI_VERSION;
	}

	OE_API OEGameEntityID CreateGameEntity(OEString name)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return INVALID_GAME_ENTITY_ID;

		std::string entityName = name;
		if (entityName.empty()) return INVALID_GAME_ENTITY_ID;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		OEGameEntityID entityID = currentRegistry->CreateGameEntity(name);
		if (!currentRegistry->IsValid(entityID) || entityID == INVALID_GAME_ENTITY_ID)
			return INVALID_GAME_ENTITY_ID;

		return entityID;
	}

	OE_API void DeleteGameEntity(OEGameEntityID id)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == INVALID_GAME_ENTITY_ID)
			return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		return id != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(id) ? currentRegistry->DeleteGameEntity(id) : void();
	}

	OE_API OEGameEntityID CopyGameEntity(OEGameEntityID originalEntityID)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || originalEntityID == INVALID_GAME_ENTITY_ID)
			return INVALID_GAME_ENTITY_ID;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;
		if (!currentRegistry->IsValid(originalEntityID)) return INVALID_GAME_ENTITY_ID;

		OEGameEntityID newGameEntityID = currentRegistry->CopyGameEntity(originalEntityID);
		if (!currentRegistry->IsValid(newGameEntityID) || newGameEntityID == INVALID_GAME_ENTITY_ID)
			return INVALID_GAME_ENTITY_ID;

		return newGameEntityID;
	}

	OE_API OEGameEntityID GetEntityIDByName(OEString name)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return INVALID_GAME_ENTITY_ID;

		std::string entityName = name;
		if (entityName.empty()) return INVALID_GAME_ENTITY_ID;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		OEGameEntityID entityID = currentRegistry->GetEntityIDByName(name);
		if (!currentRegistry->IsValid(entityID) || entityID == INVALID_GAME_ENTITY_ID)
			return INVALID_GAME_ENTITY_ID;

		return entityID;
	}

	OE_API bool GetEntityNameByID(OEGameEntityID id, char* buffer, OEUint32_t bufferSize)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == INVALID_GAME_ENTITY_ID)
			return false;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return false;

		std::string entityName = currentRegistry->GetEntityNameByID(id);

		if (entityName.empty() || entityName.size() + 1 > bufferSize)
			return false;

		if (!buffer || bufferSize == 0)
			return false;

		std::memcpy(buffer, entityName.c_str(), entityName.size() + 1);
		return true;
	}

	OE_API void SetEntityName(OEGameEntityID id, OEString newEntityName)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == INVALID_GAME_ENTITY_ID)
			return;

		std::string entityName = newEntityName;
		if (entityName.empty()) return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;
		
		return id != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(id) ? currentRegistry->SetEntityName(id, newEntityName) : void();
	}

	OE_API bool IsGameEntityValid(OEGameEntityID id)
	{
		using namespace OrionEngine::ECS;
		
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->Registry, "Registry is null!");

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;
		return currentRegistry->IsValid(id);
	}

	OE_API void AddTransformComponent(OEGameEntityID entityID, OETransformComponent component)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || entityID == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(entityID)) return;
		
		OrionEngine::ECS::OETransformComponent internalComponent;

		internalComponent.Position = glm::vec3(
			component.Position.X,
			component.Position.Y,
			component.Position.Z
		);

		internalComponent.Rotation = glm::vec3(
			component.Rotation.X,
			component.Rotation.Y,
			component.Rotation.Z
		);

		internalComponent.Scale = glm::vec3(
			component.Scale.X,
			component.Scale.Y,
			component.Scale.Z
		);

		currentRegistry->TransformComponent.AddComponent(entityID, internalComponent);
	}
	
	OE_API void DeleteTransformComponent(OEGameEntityID entityID)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		return entityID != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(entityID) ? currentRegistry->TransformComponent.DeleteComponent(entityID) : void();
	}

	OE_API void AddPhysicsComponent(OEGameEntityID entityID, OEPhysicsComponent component)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || entityID == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(entityID)) return;

		OrionEngine::ECS::OEPhysicsComponent internalComponent;

		internalComponent.Mass = component.Mass;
		internalComponent.LinearDrag = component.LinearDrag;
		internalComponent.Radius = component.Radius;
		internalComponent.IsStatic = component.IsStatic;

		internalComponent.Velocity = glm::vec3(
			component.Velocity.X,
			component.Velocity.Y,
			component.Velocity.Z
		);

		internalComponent.Accleration = glm::vec3(
			component.Acceleration.X,
			component.Acceleration.Y,
			component.Acceleration.Z
		);

		currentRegistry->PhysicsComponent.AddComponent(entityID, internalComponent);
	}

	OE_API void DeletePhysicsComponent(OEGameEntityID entityID)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		return entityID != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(entityID) ? currentRegistry->PhysicsComponent.DeleteComponent(entityID) : void();
	}

	OE_API void AddRenderableComponent(OEGameEntityID entityID, OERenderableComponent component)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || !s_ScriptingContext->AssetManager || entityID == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(entityID)) return;

		OrionEngine::ECS::OERenderableComponent internalComponent;

		internalComponent.b_Visible = component.Visible;
		internalComponent.MeshIdentifierHandle = component.MeshIdentifierHandle;
		internalComponent.MaterialIdentifierHandle = component.MaterialIdentifierHandle;
		
		OrionEngine::AssetManagement::OEAssetManager* assetManager = s_ScriptingContext->AssetManager;

		OrionEngine::AssetManagement::OEAssetIdentifierHandle meshIdentifierHandle = internalComponent.MeshIdentifierHandle;
		OrionEngine::AssetManagement::OEAssetIdentifierHandle materialIdentifierHandle = internalComponent.MaterialIdentifierHandle;

		if (meshIdentifierHandle != OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
		{
			OrionEngine::Ref<OrionEngine::AssetManagement::OEMesh> mesh = assetManager->GetAsset<OrionEngine::AssetManagement::OEMesh>(meshIdentifierHandle);
			if (mesh)
				internalComponent.Mesh = mesh->GetMesh();
		}
		else
			internalComponent.Mesh = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultMesh();

		if (materialIdentifierHandle != OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
			internalComponent.Material = assetManager->GetAsset<OrionEngine::AssetManagement::OEMaterial>(materialIdentifierHandle);
		else
			internalComponent.Material = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultMaterial();

		internalComponent.Shader = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultShader();

		currentRegistry->RenderableComponent.AddComponent(entityID, internalComponent);
	}

	OE_API void DeleteRenderableComponent(OEGameEntityID entityID)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		return entityID != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(entityID) ? currentRegistry->RenderableComponent.DeleteComponent(entityID) : void();
	}

	OE_API void AddCameraComponent(OEGameEntityID entityID, OECameraComponent component)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || entityID == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(entityID)) return;

		OrionEngine::ECS::OECameraComponent internalComponent;

		internalComponent.Camera = OrionEngine::OrionRenderer::ORCamera(
			component.Camera.Left,
			component.Camera.Right,
			component.Camera.Top,
			component.Camera.Bottom
		);

		internalComponent.Primary = component.Primary;
		internalComponent.NearClip = component.NearClip;
		internalComponent.FarClip = component.FarClip;

		currentRegistry->CameraComponent.AddComponent(entityID, internalComponent);
	}

	OE_API void DeleteCameraComponent(OEGameEntityID entityID)
	{
		using namespace OrionEngine::ECS;
		if (!s_ScriptingContext || !s_ScriptingContext->Registry)
			return;

		OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		return entityID != INVALID_GAME_ENTITY_ID && currentRegistry->IsValid(entityID) ? currentRegistry->CameraComponent.DeleteComponent(entityID) : void();
	}

	OE_API OETransformComponent GetTransformComponent(OEGameEntityID id)
	{
		OETransformComponent fallback;
		fallback.Position = { 0.0f, 0.0f, 0.0f };
		fallback.Rotation = { 0.0f, 0.0f, 0.0f };
		fallback.Scale = { 1.0f, 1.0f, 1.0f };

		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return fallback;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return fallback;

		OrionEngine::ECS::OETransformComponent baseComponent;  // To be used if the internalComponent does NOT exist
		baseComponent.Position = { 0.0f, 0.0f, 0.0f };
		baseComponent.Rotation = { 0.0f, 0.0f, 0.0f };
		baseComponent.Scale = { 1.0f, 1.0f, 1.0f };

		OrionEngine::ECS::OETransformComponent internalComponent = currentRegistry->HasComponent<OrionEngine::ECS::OETransformComponent>(id) ? *currentRegistry->GetComponent<OrionEngine::ECS::OETransformComponent>(id) : baseComponent;
		
		// Translate that internal component into the C ABI Transform Component for compatability reaons
		OETransformComponent component;

		component.Position = {
			internalComponent.Position.x,
			internalComponent.Position.y,
			internalComponent.Position.z
		};

		component.Rotation = {
			internalComponent.Rotation.x,
			internalComponent.Rotation.y,
			internalComponent.Rotation.z
		};

		component.Scale = {
			internalComponent.Scale.x,
			internalComponent.Scale.y,
			internalComponent.Scale.z
		};

		return component;
	}

	OE_API OEPhysicsComponent GetPhysicsComponent(OEGameEntityID id)
	{
		OEPhysicsComponent fallback;
		fallback.Mass = 0.0f;
		fallback.Velocity = { 0.0f, 0.0f, 0.0f };
		fallback.Acceleration = { 0.0f, 0.0f, 0.0f };
		fallback.LinearDrag = 0.0f;
		fallback.Radius = 0.0f;
		fallback.IsStatic = true;

		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return fallback;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return fallback;

		OrionEngine::ECS::OEPhysicsComponent baseComponent; 
		baseComponent.Mass = 0.0f;
		baseComponent.Velocity = { 0.0f, 0.0f, 0.0f };
		baseComponent.Accleration = { 0.0f, 0.0f, 0.0f };
		baseComponent.LinearDrag = 0.0f;
		baseComponent.Radius = 0.0f;
		baseComponent.IsStatic = true;

		OrionEngine::ECS::OEPhysicsComponent internalComponent = currentRegistry->HasComponent<OrionEngine::ECS::OEPhysicsComponent>(id) ? *currentRegistry->GetComponent<OrionEngine::ECS::OEPhysicsComponent>(id) : baseComponent;

		// Translate into C ABI struct
		OEPhysicsComponent component;
		component.Mass = internalComponent.Mass;
		component.LinearDrag = internalComponent.LinearDrag;
		component.Radius = internalComponent.Radius;
		component.IsStatic = internalComponent.IsStatic;

		component.Velocity = {
			internalComponent.Velocity.x,
			internalComponent.Velocity.y,
			internalComponent.Velocity.z
		};

		component.Acceleration = {
			internalComponent.Accleration.x,
			internalComponent.Accleration.y,
			internalComponent.Accleration.z
		};

		return component;
	}

	OE_API OERenderableComponent GetRenderableComponent(OEGameEntityID id)
	{
		OERenderableComponent fallback;
		fallback.Visible = true;
		fallback.MeshIdentifierHandle = OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;
		fallback.MaterialIdentifierHandle = OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return fallback;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return fallback;

		OrionEngine::ECS::OERenderableComponent baseComponent;
		baseComponent.b_Visible = true;
		baseComponent.MeshIdentifierHandle = OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;
		baseComponent.MaterialIdentifierHandle = OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;
		baseComponent.Shader = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultShader();
		baseComponent.Mesh = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultMesh();
		baseComponent.Material = OrionEngine::OrionRenderer::ORDefaultRendererResources::GetDefaultMaterial();

		OrionEngine::ECS::OERenderableComponent internalComponent = currentRegistry->HasComponent<OrionEngine::ECS::OERenderableComponent>(id) ? *currentRegistry->GetComponent<OrionEngine::ECS::OERenderableComponent>(id) : baseComponent;

		// Translate into C ABI struct
		OERenderableComponent component;
		component.Visible = internalComponent.b_Visible;
		component.MeshIdentifierHandle = internalComponent.MeshIdentifierHandle;
		component.MaterialIdentifierHandle = internalComponent.MaterialIdentifierHandle;

		return component;
	}

	OE_API OECameraComponent GetCameraComponent(OEGameEntityID id)
	{
		OECameraComponent fallback;
		fallback.Camera = { 0.0f, 0.0f, 0.0f, 0.0f };
		fallback.Primary = false;
		fallback.NearClip = 0.0f;
		fallback.FarClip = 1000.0f;

		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return fallback;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return fallback;

		OrionEngine::ECS::OECameraComponent baseComponent;
		baseComponent.Camera = OrionEngine::OrionRenderer::ORCamera(-10.0f, 10.0f, -10.0f, 10.0f);
		baseComponent.Primary = false;
		baseComponent.NearClip = 0.0f;
		baseComponent.FarClip = 1000.0f;

		OrionEngine::ECS::OECameraComponent internalComponent = currentRegistry->HasComponent<OrionEngine::ECS::OECameraComponent>(id) ? *currentRegistry->GetComponent<OrionEngine::ECS::OECameraComponent>(id) : baseComponent;

		// Translate into C ABI struct
		OECameraComponent component;
		component.Camera.Left = internalComponent.Camera.GetOrthoLeft();
		component.Camera.Right = internalComponent.Camera.GetOrthoRight();
		component.Camera.Top = internalComponent.Camera.GetOrthoTop();
		component.Camera.Bottom = internalComponent.Camera.GetOrthoBottom();
		component.Primary = internalComponent.Primary;
		component.NearClip = internalComponent.NearClip;
		component.FarClip = internalComponent.FarClip;

		return component;
	}

	OE_API bool HasTransformComponent(OEGameEntityID id)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return false;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return false;

		return currentRegistry->HasComponent<OrionEngine::ECS::OETransformComponent>(id);
	}

	OE_API bool HasPhysicsComponent(OEGameEntityID id)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return false;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return false;

		return currentRegistry->HasComponent<OrionEngine::ECS::OEPhysicsComponent>(id);
	}

	OE_API bool HasRenderableComponent(OEGameEntityID id)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return false;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return false;

		return currentRegistry->HasComponent<OrionEngine::ECS::OERenderableComponent>(id);
	}

	OE_API bool HasCameraComponent(OEGameEntityID id)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->Registry || id == OrionEngine::ECS::INVALID_GAME_ENTITY_ID)
			return false;

		OrionEngine::ECS::OEECSRegistry* currentRegistry = s_ScriptingContext->Registry;

		if (!currentRegistry->IsValid(id)) return false;

		return currentRegistry->HasComponent<OrionEngine::ECS::OECameraComponent>(id);
	}

	OE_API OEAssetIdentifierHandle RegisterAsset(OEAssetType type, OEString assetPath)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->AssetManager)
			return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		OrionEngine::AssetManagement::OEAssetManager* assetManager = s_ScriptingContext->AssetManager;
		std::filesystem::path path = assetPath;

		OrionEngine::AssetManagement::OEAssetIdentifierHandle assetIdentifierHandle = OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		if (path.empty() || !std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		switch (type)
		{
			case OEAssetType::OE_ASSET_TYPE_MESH:
				assetIdentifierHandle = assetManager->RegisterAsset(OrionEngine::AssetManagement::OEAssetType::Mesh, assetPath);
				break;

			case OEAssetType::OE_ASSET_TYPE_MATERIAL:
				assetIdentifierHandle = assetManager->RegisterAsset(OrionEngine::AssetManagement::OEAssetType::Material, assetPath);
				break;

			case OEAssetType::OE_ASSET_TYPE_TEXTURE:
				assetIdentifierHandle = assetManager->RegisterAsset(OrionEngine::AssetManagement::OEAssetType::Texture, assetPath);
				break;
		}

		return assetIdentifierHandle;
	}

	OE_API OEAssetIdentifierHandle LoadAsset(OEAssetType type, OEString assetPath)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->AssetManager)
			return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		OrionEngine::AssetManagement::OEAssetManager* assetManager = s_ScriptingContext->AssetManager;
		std::filesystem::path path = assetPath;

		if (path.empty() || !std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		OrionEngine::AssetManagement::OEAssetIdentifierHandle assetIdentifierHandle = assetManager->GetAssetIdentifierHandle(assetPath);

		if (assetIdentifierHandle != OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
		{
			switch (type)
			{
			case OEAssetType::OE_ASSET_TYPE_MESH:
				assetManager->LoadAsset<OrionEngine::AssetManagement::OEMesh>(assetIdentifierHandle);
				break;

			case OEAssetType::OE_ASSET_TYPE_MATERIAL:
				assetManager->LoadAsset<OrionEngine::AssetManagement::OEMaterial>(assetIdentifierHandle);
				break;

			case OEAssetType::OE_ASSET_TYPE_TEXTURE:
				assetManager->LoadAsset<OrionEngine::AssetManagement::OETexture>(assetIdentifierHandle);
				break;
			}
		}

		return assetIdentifierHandle;
	}

	OE_API bool GetAssetPath(OEAssetIdentifierHandle identifierHandle, char* buffer, OEUint32_t bufferSize)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->AssetManager || !buffer || bufferSize == 0)
			return false;

		OrionEngine::AssetManagement::OEAssetManager* assetManager = s_ScriptingContext->AssetManager;
		std::filesystem::path assetPath = assetManager->GetAssetPath(identifierHandle);

		if (assetPath.empty() || !std::filesystem::exists(assetPath) || !std::filesystem::is_regular_file(assetPath))
			return false;

		std::string pathString = assetPath.string();

		if (pathString.size() + 1 > bufferSize)
			return false;

		std::memcpy(buffer, pathString.c_str(), pathString.size() + 1);
		return true;
	}

	OE_API OEAssetIdentifierHandle GetAssetIdentifierHandle(OEString assetPath)
	{
		if (!s_ScriptingContext || !s_ScriptingContext->AssetManager)
			return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		OrionEngine::AssetManagement::OEAssetManager* assetManager = s_ScriptingContext->AssetManager;
		std::filesystem::path path = assetPath;

		if (path.empty() || !std::filesystem::exists(path) || !std::filesystem::is_regular_file(path)) return OrionEngine::AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE;

		return assetManager->GetAssetIdentifierHandle(path);
	}

	OE_API float GetDeltaTime()
	{
		return OrionEngine::OETime::OEGetDeltaTime();
	}

	OE_API double GetElapsedTime()
	{
		return OrionEngine::OETime::OEGetTime();
	}

	OE_API bool IsKeyPressed(OEKeyCodes key)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsKeyPressed(ScriptingUtils::GetStringKeyCode(key));
	}

	OE_API bool IsKeyReleased(OEKeyCodes key)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsKeyReleased(ScriptingUtils::GetStringKeyCode(key));
	}

	OE_API bool IsKeyHeld(OEKeyCodes key)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsKeyHeld(ScriptingUtils::GetStringKeyCode(key));
	}

	OE_API bool IsMouseButtonPressed(OEMouseButton mouseButton)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsMouseButtonPressed(mouseButton);
	}

	OE_API bool IsMouseButtonReleased(OEMouseButton mouseButton)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsMouseButtonReleased(mouseButton);
	}

	OE_API bool IsMouseButtonHeld(OEMouseButton mouseButton)
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->IsMouseButtonHeld(mouseButton);
	}

	OE_API OEVector2 GetMousePosition()
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		std::pair<double, double> mousePos = inputSystem->GetMousePosition();

		OEVector2 mousePosition = { mousePos.first, mousePos.second };
		return mousePosition;
	}

	OE_API OEVector2 GetMouseDelta()
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		std::pair<double, double> delta = inputSystem->GetMouseDelta();

		OEVector2 mouseDelta = { delta.first, delta.second };
		return mouseDelta;
	}

	OE_API int GetScrollDelta()
	{
		OE_CORE_ASSERT(s_ScriptingContext, "Scripting Context is null!");
		OE_CORE_ASSERT(s_ScriptingContext->InputSystem, "Input System is null!");

		OrionEngine::OEInputSystem* inputSystem = s_ScriptingContext->InputSystem;

		return inputSystem->GetScrollDelta();
	}
}