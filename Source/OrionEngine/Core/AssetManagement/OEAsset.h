#pragma once

#include "OEAssetMetadata.h"

namespace OrionEngine::AssetManagement
{
	// Base class for all engine assets
	class OEAsset
	{
	public:

		virtual ~OEAsset() noexcept = default;
		virtual void SetAssetMetadata(const OEAssetMetadata& metadata) noexcept {}

	};
}