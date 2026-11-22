#pragma once

#include <OrionEngine/OrionRenderer/IBL/ORIBLData.h>
#include <OrionEngine/OrionRenderer/IBL/ORIBLSettings.h>
#include <OrionEngine/Core/OELogging.h>

#include <filesystem>
#include <utility>
#include <algorithm>
#include <cctype>

namespace OrionEngine
{
	struct OEEnvironmentData
	{
		OrionRenderer::ORIBLData EnvironmentIBLData;
		OrionRenderer::ORIBLSettings EnvironmentIBLSettings;
		std::filesystem::path EnvironmentHDRTexturePath;
	};

	class OEEnvironment
	{
	public:

		void SetEnvironmentIBLData(OrionRenderer::ORIBLData IBLdata)
		{
			m_EnvironmentData.EnvironmentIBLData = std::move(IBLdata);
		}

		void SetEnvironmentHDRTexturePath(const std::filesystem::path& environmentHDRTexturePath)
		{
			if (environmentHDRTexturePath.empty() || !std::filesystem::is_regular_file(environmentHDRTexturePath))
				return;

			std::string extension = environmentHDRTexturePath.extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

			if (extension != ".hdr")
			{
				ORION_ENGINE_ERROR("Unsupported extension: ", extension);
				return;
			}

			if (m_EnvironmentData.EnvironmentHDRTexturePath == environmentHDRTexturePath)
				return;

			m_EnvironmentData.EnvironmentHDRTexturePath = environmentHDRTexturePath;
			MarkIBLDirty();
		}

		const OrionRenderer::ORIBLData& GetEnvironmentIBLData() const noexcept { return m_EnvironmentData.EnvironmentIBLData; }
		
		const OrionRenderer::ORIBLSettings& GetEnvironmentIBLSettings() const noexcept { return m_EnvironmentData.EnvironmentIBLSettings; }
		OrionRenderer::ORIBLSettings& GetEnvironmentIBLSettings() noexcept { return m_EnvironmentData.EnvironmentIBLSettings; }

		const std::filesystem::path& GetEnvironmentHDRTexturePath() const noexcept { return m_EnvironmentData.EnvironmentHDRTexturePath; }

		const OEEnvironmentData& GetEnvironmentData() const noexcept { return m_EnvironmentData; }

		void MarkIBLDirty() noexcept { m_IBLDirty = true; }
		void ClearIBLDirty() noexcept { m_IBLDirty = false; }
		bool IsIBLDirty() const noexcept { return m_IBLDirty; }

	private:

		OEEnvironmentData m_EnvironmentData;
		bool m_IBLDirty = false;
	};
}