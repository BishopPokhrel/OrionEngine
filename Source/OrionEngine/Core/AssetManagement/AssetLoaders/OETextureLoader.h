#pragma once

#include "OEAssetLoader.h"

#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OETexture.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::AssetManagement
{
	class OETextureLoader : public OEAssetLoader
	{
	public:

		OETextureLoader() = default;
		~OETextureLoader() = default;
	
		Ref<OEAsset> LoadAsset(const std::filesystem::path& assetPath) override;
	};
}