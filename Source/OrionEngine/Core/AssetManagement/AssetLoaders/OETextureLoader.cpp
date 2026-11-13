#include "OETextureLoader.h"

#include <string>
#include <algorithm>
#include <cctype>
#include <utility>

#include <OrionEngine/Core/OELogging.h>

#include <stb_image.h>

namespace OrionEngine::AssetManagement
{
	Ref<OEAsset> OETextureLoader::LoadAsset(const std::filesystem::path& assetPath)
	{
		if (assetPath.empty() || !std::filesystem::is_regular_file(assetPath))
			return nullptr;

		std::string extension = assetPath.extension().string();
		std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

		const char* supportedExtensions[] = {
			".png", ".jpeg", ".jpg", ".bmp", ".hdr"
		};

		if (std::find(std::begin(supportedExtensions), std::end(supportedExtensions), extension) == std::end(supportedExtensions))
		{
			ORION_ENGINE_ERROR("Texture extension ", assetPath, " for texture ", assetPath, " is not supported.");
			return nullptr;
		}

		// Attempt to load the texture path
		Ref<OETexture> texture = CreateRef<OETexture>();

		const bool isHDR = stbi_is_hdr(assetPath.string().c_str()) != 0;

		if (isHDR)
		{
			// HDR implementation
			int width = 0, height = 0, channels = 0;

			std::unique_ptr<float, decltype(&stbi_image_free)> imageTextureData(stbi_loadf(assetPath.string().c_str(), &width, &height, &channels, 0), stbi_image_free);

			if (!imageTextureData)
			{
				ORION_ENGINE_ERROR("Failed to load texture at path ", assetPath.string(), " due to reason ", stbi_failure_reason());
				return nullptr;
			}

			if (width <= 0 || height <= 0 || channels <= 0)
			{
				ORION_ENGINE_ERROR("Invalid image dimensions or channel count: ", assetPath);
				return nullptr;
			}

			texture->SetWidth(static_cast<uint32_t>(width));
			texture->SetHeight(static_cast<uint32_t>(height));
			texture->SetChannels(static_cast<uint32_t>(channels));
			texture->SetImageHDR(true);

			const size_t imageSize = static_cast<size_t>(width) * height * channels;
			std::vector<float> imagePixel(imageTextureData.get(), imageTextureData.get() + imageSize);

			texture->SetHDRPixelData(std::move(imagePixel));
		}
		else
		{
			// LDR implementation
			int width = 0, height = 0, channels = 0;

			std::unique_ptr<stbi_uc, decltype(&stbi_image_free)> imageTextureData(stbi_load(assetPath.string().c_str(), &width, &height, &channels, 0), stbi_image_free);

			if (!imageTextureData)
			{
				ORION_ENGINE_ERROR("Failed to texture at path ", assetPath.string(), " due to reason ", stbi_failure_reason());
				return nullptr;
			}

			if (width <= 0 || height <= 0 || channels <= 0)
			{
				ORION_ENGINE_ERROR("Invalid image dimensions or channel count: ", assetPath);
				return nullptr;
			}

			texture->SetWidth(static_cast<uint32_t>(width));
			texture->SetHeight(static_cast<uint32_t>(height));
			texture->SetChannels(static_cast<uint32_t>(channels));
			texture->SetImageHDR(false);

			const size_t imageSize = static_cast<size_t>(width) * height * channels;
			std::vector<uint8_t> imagePixel(imageTextureData.get(), imageTextureData.get() + imageSize);

			texture->SetPixelData(std::move(imagePixel));
		}

		return texture;
	}
}
