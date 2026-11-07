#include "OEMaterialLoader.h"

#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OELogging.h>

#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMaterial.h>

#include <algorithm>
#include <cctype>
#include <fstream>

namespace OrionEngine::AssetManagement
{
	static const char* GetStringAssetType(OEAssetType type)
	{
		switch (type)
		{
			case OEAssetType::None:         return "Unknown";
			case OEAssetType::Mesh:         return "Mesh";
			case OEAssetType::Texture:      return "Texture";
			case OEAssetType::Material:     return "Material";
			case OEAssetType::Shader:       return "Shader";
			case OEAssetType::Script:       return "Script";
			default:                        return "";
		}
	}

	static OEAssetType GetEnumAssetTypeFromString(const std::string& name)
	{
		if (name == "Mesh")
			return OEAssetType::Mesh;
		else if (name == "Texture")
			return OEAssetType::Texture;
		else if (name == "Material")
			return OEAssetType::Material;
		else if (name == "Shader")
			return OEAssetType::Shader;
		else if (name == "Script")
			return OEAssetType::Script;
		else
			return OEAssetType::None;
	}

	Ref<OEAsset> OEMaterialLoader::LoadAsset(const std::filesystem::path& assetPath)
	{
		if (assetPath.empty() || !std::filesystem::is_regular_file(assetPath))
			return nullptr;

		std::string extension = assetPath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

		if (extension != ".oemat")
			return nullptr;

		Ref<OEMaterial> material = CreateRef<OEMaterial>();

		if (!DeserializeMaterialFile(assetPath, material))
			return nullptr;

		return material;
	}

	// ----------------------------
	// PRIVATE HELPERS 
	// ----------------------------

	bool OEMaterialLoader::CreateMaterialFile(const std::filesystem::path& projectPath, const std::string& materialName)
	{
		if (!std::filesystem::is_directory(projectPath))
			return false;

		if (materialName.empty() || materialName.find_first_of("/\\") != std::string::npos)
			return false;

		try
		{
			std::filesystem::path assetsPath = projectPath / "Assets";

			if (!std::filesystem::is_directory(assetsPath))
				return false;

			std::filesystem::path materialPath = assetsPath / (materialName + ".oemat");

			if (std::filesystem::exists(materialPath))
				return false;

			Ref<OEMaterial> material = CreateRef<OEMaterial>();

			if (!SerializeMaterialFile(materialPath, material))
			{
				ORION_ENGINE_ERROR("Failed to serialize material at path ", materialPath.string());
				return false;
			}

			ORION_ENGINE_INFO("Successfully created Material file ", materialPath.stem().string(), " at path ", materialPath);
			return true;
		}
		catch (const std::filesystem::filesystem_error& err)
		{
			ORION_ENGINE_ERROR("Filesystem error: ", err.code().message());
			return false;
		}
	}

	bool OEMaterialLoader::SerializeMaterialFile(const std::filesystem::path& path, const Ref<OEMaterial>& material)
	{
		OE_CORE_ASSERT(material, "Material is null!");

		if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
			return false;

		std::string extension = path.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

		if (extension != ".oemat")
			return false;
		
		YAML::Emitter out;

		out << YAML::Key << "Material";
		out << YAML::Value << YAML::BeginMap;

		const OEMaterialData& materialData = material->GetMaterialData();

		out << YAML::Key << "Albedo Color";
		out << YAML::Value << YAML::Flow << YAML::BeginSeq
			<< materialData.AlbedoColor.x
			<< materialData.AlbedoColor.y
			<< materialData.AlbedoColor.z
			<< materialData.AlbedoColor.a << YAML::EndSeq;

		out << YAML::Key << "Metallic";
		out << YAML::Value << materialData.Metallic;

		out << YAML::Key << "Roughness";
		out << YAML::Value << materialData.Roughness;

		out << YAML::Key << "Ambient Occlusion";
		out << YAML::Value << materialData.AmbientOcclusion;

		out << YAML::Key << "Asset Metadata";
		out << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "Asset Identifier Handle";
		out << YAML::Value << materialData.MaterialAssetMetadata.IdentifierHandle;

		out << YAML::Key << "Asset Type";
		out << YAML::Value << GetStringAssetType(materialData.MaterialAssetMetadata.Type);
		
		out << YAML::Key << "Path";
		out << YAML::Value << materialData.MaterialAssetMetadata.Path.string();

		out << YAML::EndMap;

		out << YAML::EndMap;

		// Save to disk
		std::ofstream outputFile(path);

		if (!outputFile.is_open())
			return false;

		outputFile << out.c_str();

		if (!outputFile.good() || !out.good())
			return false; 

		return true;
	}

	bool OEMaterialLoader::DeserializeMaterialFile(const std::filesystem::path& path, const Ref<OEMaterial>& material)
	{
		OE_CORE_ASSERT(material, "Material is null!");

		if (!std::filesystem::exists(path) || !std::filesystem::is_regular_file(path))
			return false;

		std::string extension = path.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

		if (extension != ".oemat")
			return false;

		YAMLNode data;

		try
		{
			data = YAML::LoadFile(path.string());
		}
		catch (const YAML::Exception& e)
		{
			ORION_ENGINE_ERROR("YAML Exception: ", e.what());
			return false;
		}

		if (!data["Material"])
			return false;

		YAMLNode materialNode = data["Material"];

		YAMLNode albedoColorNode = materialNode["Albedo Color"];
		YAMLNode metallicNode = materialNode["Metallic"];
		YAMLNode roughnessNode = materialNode["Roughness"];
		YAMLNode ambientOcclusionNode = materialNode["Ambient Occlusion"];

		glm::vec4 albedoColorMaterial(
			albedoColorNode[0].as<float>(),
			albedoColorNode[1].as<float>(),
			albedoColorNode[2].as<float>(),
			albedoColorNode[3].as<float>()
		);

		float metallicMeterial = metallicNode.as<float>();
		float roughnessMaterial = roughnessNode.as<float>();
		float ambientOcclusionMaterial = ambientOcclusionNode.as<float>();

		YAMLNode assetMetadata = materialNode["Asset Metadata"];

		YAMLNode assetIdentifierHandleNode = assetMetadata["Asset Identifier Handle"];
		YAMLNode assetTypeNode = assetMetadata["Asset Type"];
		YAMLNode assetPathNode = assetMetadata["Path"];

		OEAssetIdentifierHandle assetIdentifierHandleMaterial = assetIdentifierHandleNode.as<OEAssetIdentifierHandle>();
		OEAssetType assetTypeMaterial = GetEnumAssetTypeFromString(assetTypeNode.as<std::string>());
		std::filesystem::path assetPathMaterial = std::filesystem::path(assetPathNode.as<std::string>());

		material->SetAlbedoColor(albedoColorMaterial);
		material->SetMetallic(metallicMeterial);
		material->SetRoughness(roughnessMaterial);
		material->SetAmbientOcclusion(ambientOcclusionMaterial);

		if (assetIdentifierHandleMaterial != INVALID_ASSET_IDENTIFIER_HANDLE)
		{
			OEAssetMetadata assetMetadata;
			assetMetadata.IdentifierHandle = assetIdentifierHandleMaterial;
			assetMetadata.Type = assetTypeMaterial;
			assetMetadata.Path = assetPathMaterial;

			material->SetMaterialAssetMetadata(assetMetadata);
		}

		return true;
	}
}