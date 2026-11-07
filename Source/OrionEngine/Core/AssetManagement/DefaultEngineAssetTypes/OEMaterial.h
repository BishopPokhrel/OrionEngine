#pragma once

#include <OrionEngine/Core/AssetManagement/OEAssetMetadata.h>
#include <OrionEngine/Core/AssetManagement/OEAsset.h>

#include <glm/glm.hpp>

namespace OrionEngine::AssetManagement
{
	struct OEMaterialData
	{
		glm::vec4 AlbedoColor{1.0f}; 
		float Metallic = 0.0f; 
		float Roughness = 0.5f; 
		float AmbientOcclusion = 1.0f; 
		
		OEAssetMetadata MaterialAssetMetadata;
	};

	class OEMaterial : public OEAsset
	{
	public:

		OEMaterial() = default;
		~OEMaterial() = default;

		// Setters

		void SetAlbedoColor(const glm::vec4& color);
		void SetMetallic(float metallicValue);
		void SetRoughness(float roughnessValue);
		void SetAmbientOcclusion(float ambientOcclusionValue);
		void SetMaterialAssetMetadata(OEAssetMetadata metadata);

		// Getters
		const OEMaterialData& GetMaterialData() const noexcept { return m_MaterialData; }

	private:

		OEMaterialData m_MaterialData;
	};
}