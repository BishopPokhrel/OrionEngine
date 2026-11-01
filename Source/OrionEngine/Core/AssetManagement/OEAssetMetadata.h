#pragma once

#include "OEAssetIdentifierHandle.h"

#include <filesystem>

namespace OrionEngine::AssetManagement
{
	enum class OEAssetType
	{
		None = 0,
		Mesh,
		Texture,
		Material,
		Shader, 
		Script
	};

	struct OEAssetMetadata
	{
		OEAssetIdentifierHandle IdentifierHandle = INVALID_ASSET_IDENTIFIER_HANDLE;
		OEAssetType Type = OEAssetType::None;
		std::filesystem::path Path;
	};
}