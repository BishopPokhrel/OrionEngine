#pragma once

#include <OrionEngine/Core/AssetManagement/OEAsset.h>
#include <OrionEngine/Core/OERef.h>

#include <filesystem>

namespace OrionEngine::AssetManagement
{
	// Base class for all asset loaders 
	class OEAssetLoader
	{
	public:

		virtual ~OEAssetLoader() = default;

		virtual Ref<OEAsset> LoadAsset(const std::filesystem::path& assetPath) = 0;
	};
}