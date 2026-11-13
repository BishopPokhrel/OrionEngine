#pragma once

#include <filesystem>
#include <cstdint>
#include <vector>
#include <string>

#include <OrionEngine/Core/AssetManagement/OEAssetMetadata.h>
#include <OrionEngine/Core/AssetManagement/OEAsset.h>

namespace OrionEngine::AssetManagement
{
	struct OETextureData
	{
		uint32_t Width = 0;
		uint32_t Height = 0;
		uint32_t Channels = 0;

		bool IsImageHDR = false;

		std::vector<uint8_t> PixelData;
		std::vector<float> HDRPixelData;

		OEAssetMetadata TextureAssetMetadata;
	};

	class OETexture : public OEAsset
	{
	public:

		OETexture() = default;
		~OETexture() = default;

		bool SetTexturePath(const std::filesystem::path& projectPath, const std::string& textureName, const std::string& textureFileExtension);
		void SetWidth(uint32_t width);
		void SetHeight(uint32_t height);
		void SetChannels(uint32_t channels);
		void SetImageHDR(bool isHDR);

		void SetPixelData(std::vector<uint8_t>&& pixelData);
		void SetHDRPixelData(std::vector<float>&& hdrPixelData);
		void SetTextureAssetMetadata(OEAssetMetadata metadata);

		const bool IsImageHDR() const noexcept { return m_TextureData.IsImageHDR; }
		const std::filesystem::path& GetTexturePath() const noexcept { return m_TextureData.TextureAssetMetadata.Path; }
		const OETextureData& GetTextureData() const noexcept { return m_TextureData; }

	private:

		OETextureData m_TextureData;
	};
}
