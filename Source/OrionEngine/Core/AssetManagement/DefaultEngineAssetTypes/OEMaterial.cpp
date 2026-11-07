#include "OEMaterial.h"

namespace OrionEngine::AssetManagement
{
	// Metallic = 0.0f - 1.0f
	// Roughness = 0.0f - 1.0f
	// Ambient Occlusion = 0.0f - 1.0f
	// Albedo Color = Each channel 0.0f - 1.0f

	void OEMaterial::SetAlbedoColor(const glm::vec4& color)
	{
		if ((color.x < 0.0f || color.x > 1.0f || !std::isfinite(color.x)) || (color.y < 0.0f || color.y > 1.0f || !std::isfinite(color.y)) || (color.z < 0.0f || color.z > 1.0f || !std::isfinite(color.z)) || (color.a < 0.0f || color.a > 1.0f) || !std::isfinite(color.a))
			return;

		m_MaterialData.AlbedoColor = color;
	}

	void OEMaterial::SetMetallic(float metallicValue)
	{
		if (metallicValue < 0.0f || metallicValue > 1.0f || !std::isfinite(metallicValue))
			return;

		m_MaterialData.Metallic = metallicValue;
	}

	void OEMaterial::SetRoughness(float roughnessValue)
	{
		if (roughnessValue < 0.0f || roughnessValue > 1.0f || !std::isfinite(roughnessValue))
			return;

		m_MaterialData.Roughness = roughnessValue;
	}

	void OEMaterial::SetAmbientOcclusion(float ambientOcclusionValue)
	{
		if (ambientOcclusionValue < 0.0f || ambientOcclusionValue > 1.0f || !std::isfinite(ambientOcclusionValue))
			return;

		m_MaterialData.AmbientOcclusion = ambientOcclusionValue;
	}

	void OEMaterial::SetMaterialAssetMetadata(OEAssetMetadata metadata)
	{
		m_MaterialData.MaterialAssetMetadata = metadata;
	}
}