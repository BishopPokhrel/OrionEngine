#include "OETexture.h"

#include <OrionEngine/Core/OELogging.h>

#include <algorithm>
#include <cctype>

namespace OrionEngine::AssetManagement
{
	bool OETexture::SetTexturePath(const std::filesystem::path& projectPath, const std::string& textureName, const std::string& textureFileExtension)
	{
		try
		{
			if (projectPath.empty())
			{
				ORION_ENGINE_ERROR("Project path is empty!");
				return false;
			}

			if (textureName.empty() || textureName.find_first_of("/\\") != std::string::npos)
			{
				ORION_ENGINE_ERROR("Invalid texture name!");
				return false;
			}

			if (textureFileExtension.empty() || textureFileExtension.front() != '.')
			{
				ORION_ENGINE_ERROR("Invalid file extension ", textureFileExtension);
				return false;
			}

			std::string projectExtension = projectPath.extension().string();
			std::transform(projectExtension.begin(), projectExtension.end(), projectExtension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

			if (projectExtension != ".oeproj")
			{
				ORION_ENGINE_ERROR("Extension is not .oeproj!");
				return false;
			}

			const std::filesystem::path assetsPath = projectPath.parent_path() / "Assets";

			if (!std::filesystem::is_directory(assetsPath))
			{
				ORION_ENGINE_ERROR("Assets directory does not exist!");
				return false;
			}

			const char* supportedExtensions[] = {
				".png", ".jpeg", ".jpg", ".bmp", ".hdr"
			};

			std::string textureExtension = textureFileExtension;
			std::transform(textureExtension.begin(), textureExtension.end(), textureExtension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

			const std::string_view textureExt = textureExtension;
			const std::filesystem::path textureDirPath = assetsPath / "Textures";

			if (std::find(std::begin(supportedExtensions), std::end(supportedExtensions), textureExt) == std::end(supportedExtensions))
			{
				ORION_ENGINE_ERROR("Texture extension ", textureFileExtension, " for texture ", textureName, " is not supported.");
				return false;
			}

			if (!std::filesystem::is_directory(textureDirPath))
				std::filesystem::create_directories(textureDirPath);

			const std::filesystem::path texturePath = textureDirPath / (textureName + textureFileExtension);

			m_TextureData.TextureAssetMetadata.Path = texturePath;

			return true;
		}
		catch (const std::filesystem::filesystem_error& err)
		{
			ORION_ENGINE_ERROR("Filesystem error: ", err.code().message());
			return false;
		}
	}

	void OETexture::SetWidth(uint32_t width)
	{
		m_TextureData.Width = width;
	}

	void OETexture::SetHeight(uint32_t height)
	{
		m_TextureData.Height = height;
	}

	void OETexture::SetChannels(uint32_t channels)
	{
		m_TextureData.Channels = channels;
	}

	void OETexture::SetImageHDR(bool isHDR)
	{
		m_TextureData.IsImageHDR = isHDR;
	}

	void OETexture::SetPixelData(std::vector<uint8_t>&& pixelData)
	{
		m_TextureData.PixelData = std::move(pixelData);
	}

	void OETexture::SetHDRPixelData(std::vector<float>&& hdrPixelData)
	{
		m_TextureData.HDRPixelData = std::move(hdrPixelData);
	}

	void OETexture::SetTextureAssetMetadata(OEAssetMetadata metadata)
	{
		m_TextureData.TextureAssetMetadata = metadata;
	}
}