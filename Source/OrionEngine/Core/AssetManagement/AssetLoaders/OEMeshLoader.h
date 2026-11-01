#pragma once

#include "OEAssetLoader.h"

#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMesh.h>

#include <glm/glm.hpp>

namespace OrionEngine::AssetManagement
{
	class OEMeshLoader : public OEAssetLoader
	{
	public:

		Ref<OEAsset> LoadAsset(const std::filesystem::path& assetPath) override;

	private:

		struct OEMeshVertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 UV;
		};
	};
}