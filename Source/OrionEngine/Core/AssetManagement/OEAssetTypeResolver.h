#pragma once

#include "DefaultEngineAssetTypes/OEMesh.h"
#include "DefaultEngineAssetTypes/OEMaterial.h"
#include "DefaultEngineAssetTypes/OETexture.h"

#include "OEAssetMetadata.h"

namespace OrionEngine::AssetManagement
{
	template<typename T>
	struct OEAssetTypeResolver;

	template<>
	struct OEAssetTypeResolver<OEMesh>
	{
		static constexpr OEAssetType Type = OEAssetType::Mesh;
	};

	template<>
	struct OEAssetTypeResolver<OEMaterial>
	{
		static constexpr OEAssetType Type = OEAssetType::Material;
	};

	template<>
	struct OEAssetTypeResolver<OETexture>
	{
		static constexpr OEAssetType Type = OEAssetType::Texture;
	};
}