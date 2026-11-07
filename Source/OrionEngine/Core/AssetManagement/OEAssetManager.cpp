#include "OEAssetManager.h"

#include <utility>

namespace OrionEngine::AssetManagement
{
	OEAssetIdentifierHandle OEAssetManager::RegisterAsset(OEAssetType assetType, const std::filesystem::path& assetPath)
	{
		if (assetType == OEAssetType::None || assetPath.empty() || m_NextAssetIdentifierHandle == INVALID_ASSET_IDENTIFIER_HANDLE)
			return INVALID_ASSET_IDENTIFIER_HANDLE;

		const OEAssetIdentifierHandle existingAssetIdentifierHandle = GetAssetIdentifierHandle(assetPath);
		if (existingAssetIdentifierHandle != INVALID_ASSET_IDENTIFIER_HANDLE)
			return existingAssetIdentifierHandle;

		OEAssetMetadata metadata;
		metadata.IdentifierHandle = m_NextAssetIdentifierHandle++;
		metadata.Type = assetType;
		metadata.Path = assetPath;

		auto [iterator, isRegistered] = m_AssetRegistry.emplace(metadata.IdentifierHandle, metadata);
		if (!isRegistered)
			return INVALID_ASSET_IDENTIFIER_HANDLE;

		return metadata.IdentifierHandle;
	}

	OEAssetIdentifierHandle OEAssetManager::GetAssetIdentifierHandle(const std::filesystem::path& assetPath) const
	{
		if (assetPath.empty())
			return INVALID_ASSET_IDENTIFIER_HANDLE;

		for (const auto& [handle, metadata] : m_AssetRegistry)
		{
			if (metadata.Path == assetPath)
				return handle;
		}

		return INVALID_ASSET_IDENTIFIER_HANDLE;
	}

	std::filesystem::path OEAssetManager::GetAssetPath(OEAssetIdentifierHandle identifierHandle) const
	{
		if (identifierHandle == INVALID_ASSET_IDENTIFIER_HANDLE)
			return {};

		auto iterator = m_AssetRegistry.find(identifierHandle);

		if (iterator != m_AssetRegistry.end())
			return iterator->second.Path;

		return {};
	}

	bool OEAssetManager::UnloadAsset(OEAssetIdentifierHandle identifierHandle)
	{
		if (identifierHandle == INVALID_ASSET_IDENTIFIER_HANDLE)
			return false;

		auto iterator = m_LoadedAssets.find(identifierHandle);

		if (iterator == m_LoadedAssets.end())
			return false;

		m_LoadedAssets.erase(iterator);
		return true;
	}

	bool OEAssetManager::RegisterLoader(OEAssetType assetType, Scope<OEAssetLoader> loader)
	{
		if (assetType == OEAssetType::None || !loader)
			return false;

		auto [iterator, isRegistered] = m_AssetLoaders.emplace(assetType, std::move(loader));

		return isRegistered;
	}
}