#pragma once

#include "OEAssetLoader.h"

#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMaterial.h>

#include <yaml-cpp/yaml.h>

namespace OrionEngine::AssetManagement
{
	using YAMLNode = YAML::Node;

	class OEMaterialLoader : public OEAssetLoader
	{
	public:

		OEMaterialLoader() = default;
		~OEMaterialLoader() = default;

		Ref<OEAsset> LoadAsset(const std::filesystem::path& assetPath) override;

	private:

		bool CreateMaterialFile(const std::filesystem::path& projectPath, const std::string& materialName);
		bool SerializeMaterialFile(const std::filesystem::path& path, const Ref<OEMaterial>& material);
		bool DeserializeMaterialFile(const std::filesystem::path& path, const Ref<OEMaterial>& material);
	};
} 