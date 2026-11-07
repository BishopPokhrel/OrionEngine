#pragma once

#include "OEAssetIdentifierHandle.h"
#include "OEAsset.h"
#include "OEAssetMetadata.h"
#include "OEAssetTypeResolver.h"

#include "AssetLoaders/OEAssetLoader.h"

#include <OrionEngine/Core/OERef.h>

#include <filesystem>
#include <unordered_map>

namespace OrionEngine::AssetManagement
{
	class OEAssetManager
	{
	public:

		// Orion Engine will only load assets that have been registered and and have a valid asset identifier handle
		// Therefore, Orion Engine's OEAssetManager class will expose a public API function that allows to register assets.
		// An asset can only be loaded if it is registered correctly

		OEAssetIdentifierHandle RegisterAsset(OEAssetType assetType, const std::filesystem::path& assetPath);
		OEAssetIdentifierHandle GetAssetIdentifierHandle(const std::filesystem::path& assetPath) const;
        std::filesystem::path GetAssetPath(OEAssetIdentifierHandle idenifierHandle) const;

		bool RegisterLoader(OEAssetType assetType, Scope<OEAssetLoader> loader);

		// Load Asset
		template<typename AssetType>
		inline Ref<AssetType> LoadAsset(OEAssetIdentifierHandle identifierHandle);

		bool UnloadAsset(OEAssetIdentifierHandle identifierHandle);

	private:

		std::unordered_map<OEAssetIdentifierHandle, OEAssetMetadata> m_AssetRegistry;
		std::unordered_map<OEAssetIdentifierHandle, Ref<OEAsset>> m_LoadedAssets;
		std::unordered_map<OEAssetType, Scope<OEAssetLoader>> m_AssetLoaders;

		OEAssetIdentifierHandle m_NextAssetIdentifierHandle = 1;
	};


    template<typename AssetType>
    inline Ref<AssetType> OEAssetManager::LoadAsset(OEAssetIdentifierHandle identifierHandle)
    {
        if (identifierHandle == INVALID_ASSET_IDENTIFIER_HANDLE)
            return nullptr;

        // Find the asset's metadata.
        auto metadataIterator = m_AssetRegistry.find(identifierHandle);

        if (metadataIterator == m_AssetRegistry.end())
            return nullptr;

        // Verify the requested asset type.
        constexpr OEAssetType requiredAssetType = OEAssetTypeResolver<AssetType>::Type;

        if (metadataIterator->second.Type != requiredAssetType)
            return nullptr;

        // Check if the asset is already loaded.
        auto assetIterator = m_LoadedAssets.find(identifierHandle);

        if (assetIterator != m_LoadedAssets.end())
            return std::static_pointer_cast<AssetType>(assetIterator->second);

        // Find the appropriate asset loader.
        auto loaderIterator = m_AssetLoaders.find(requiredAssetType);

        if (loaderIterator == m_AssetLoaders.end())
            return nullptr;

        Ref<OEAsset> asset = loaderIterator->second->LoadAsset(metadataIterator->second.Path);

        if (!asset)
            return nullptr;

        // Cache the loaded asset.
        m_LoadedAssets.emplace(identifierHandle, asset);

        return std::static_pointer_cast<AssetType>(asset);
    }
}