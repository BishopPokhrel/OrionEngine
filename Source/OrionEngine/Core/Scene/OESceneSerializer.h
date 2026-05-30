#pragma once

#include <OrionEngine/Core/OERef.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include "OEScene.h"

namespace OrionEngine
{
	class OESceneSerializer
	{
	public:

		bool CreateOEProjectDirectory(const std::string& projectName);
		bool SerializeEntities(YAML::Emitter& out, const std::filesystem::path& path);
		bool SerializeScene();

		bool DoesProjectExist(const std::string& projectName);

	private:

		OEScene m_Scene; // Allocate it on the stack so we dont get any seg fault or nullptr errors
		
		struct DirectoriesCreated
		{
			bool AssetDirectoryCreated = false;
			bool ScriptsDirectoryCreated = false;
			bool ProjectDirectoryCreated = false; 
			
			struct FilesCreated
			{
				bool ProjectFileCreated = false;
				bool SceneFileCreated = false;
			};
			
			struct FailedToCreateDirsOrFiles
			{
				// FILES
				bool FailedToCreateProjectFile = false;
				bool FailedToCreateSceneFile = false;

				// FOLDERS
				bool FailedToCreateAssetsDirectory = false;
				bool FailedToCreateScriptsDirectory = false;
				bool FailedToCreateProjectDirectory = false; 
			};

			Scope<FilesCreated> pFilesCreated = CreateScope<FilesCreated>();
			Scope<FailedToCreateDirsOrFiles> pFailedToCreateDirsOrFiles = CreateScope<FailedToCreateDirsOrFiles>();
		};

		DirectoriesCreated m_DirectoriesCreated;
		
		std::filesystem::path m_ProjectPath;
		std::filesystem::path m_ScenePath;
	};
}