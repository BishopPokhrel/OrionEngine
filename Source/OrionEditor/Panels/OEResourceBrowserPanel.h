#pragma once

#include <OrionEngine/Core/AssetManagement/OEAssetManager.h>
#include <OrionEngine/Core/Scene/OEScene.h>

#include <filesystem>
#include <vector>

namespace OrionEngine::OrionEditor
{
	class OEResourceBrowserPanel
	{
	public:

		explicit OEResourceBrowserPanel(AssetManagement::OEAssetManager* assetManager, OEScene* scene)
			: m_AssetManager(assetManager), m_Scene(scene)
		{ }

		bool DrawProjectHierarchy();
		bool DrawResourceBrowserPanel();

		void SetProjectPath(const std::filesystem::path& path);

	private:

		struct OERBPDTDPDN // OERBPDTDPDN = Orion Engine Resource Browser Panel Directory Toolbar Directory Path Data Needed
		{
			char CurrentDirectoryPathBuffer[1024];
		};

		struct OERBPDCDN // OERBPDCDN = Orion Engine Resource Browser Panel Directory Contents Data Needed 
		{
			bool IsFolderSelected = false;
			bool IsFileSelected = false;
		};

	private:

		bool RefreshDirectoryEntries();
		void UpdateDirectoryPath();

		bool DrawDirectoryContents();
		bool DrawDirectoryToolbar();

		bool HandleFileDrop(const std::filesystem::path& filePath);
		bool ImportAsset(const std::filesystem::path& filePath);

	private:

		AssetManagement::OEAssetManager* m_AssetManager = nullptr;
		OEScene* m_Scene = nullptr;

		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_ScenePath;
		std::filesystem::path m_CurrentDirectory;
		std::vector<std::filesystem::directory_entry> m_DirectoryEntries;

		OERBPDTDPDN m_OERBPDTDPDN;
		OERBPDCDN m_OERBPDCDN;
	};
}