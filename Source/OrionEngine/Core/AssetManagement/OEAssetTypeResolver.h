#pragma once

#include "DefaultEngineAssetTypes/OEMesh.h"
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
}