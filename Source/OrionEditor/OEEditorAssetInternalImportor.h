#pragma once

#include <OrionEngine/Core/Scene/OEScene.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/AssetManagement/OEAssetManager.h>
#include <OrionEngine/Core/OEAssert.h>

#include <filesystem>
#include <cctype>
#include <algorithm>

namespace OrionEngine::OrionEditor
{
	inline bool HandleAssetDrop(const std::filesystem::path& filePath, OEScene* scene, AssetManagement::OEAssetManager* assetManager)
	{
		OE_CORE_ASSERT(scene, "Scene is null!");
		OE_CORE_ASSERT(assetManager, "Asset Manager is null!");

		if (filePath.empty() || !std::filesystem::exists(filePath) || !std::filesystem::is_regular_file(filePath))
			return false;

		// Support OBJ (mesh) assets for now
		std::string extension = filePath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) {return static_cast<char>(std::tolower(character)); });

		if (extension != ".obj")
			return false;

		// Register and load the asset

		const AssetManagement::OEAssetIdentifierHandle assetIdentifierHandle = assetManager->RegisterAsset(AssetManagement::OEAssetType::Mesh, filePath);

		if (assetIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
			return false;

		const Ref<AssetManagement::OEMesh> mesh = assetManager->LoadAsset<AssetManagement::OEMesh>(assetIdentifierHandle);

		if (!mesh) return false;

		ECS::OEGameEntityID assetEntityID = scene->GetRegistry().CreateGameEntity(filePath.stem().string());

		if (assetEntityID == ECS::INVALID_GAME_ENTITY_ID)
			return false;

		// Create the Transform Component
		ECS::OETransformComponent assetTransform;
		assetTransform.Position = { 0.0f, 0.0f, 0.0f };
		assetTransform.Rotation = { 0.0f, 0.0f, 0.0f };
		assetTransform.Scale = { 1.0f, 1.0f, 1.0f };

		// Create the Renderable Component
		ECS::OERenderableComponent assetRenderable;
		assetRenderable.b_Visible = true;
		assetRenderable.MeshIdenifierHandle = assetIdentifierHandle;
		assetRenderable.Mesh = mesh->GetMesh();

		// Attach the components to the game entity
		scene->GetRegistry().TransformComponent.AddComponent(assetEntityID, assetTransform);
		scene->GetRegistry().RenderableComponent.AddComponent(assetEntityID, assetRenderable);

		return true;
	}
}