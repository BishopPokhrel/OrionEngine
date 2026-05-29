#include "OESceneSerializer.h"
#include "OEScene.h"

#include <filesystem>
#include <fstream>

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	bool OESceneSerializer::CreateOEProjectDirectory(const std::string& projectName)
	{		
		// ASSERT POINTER VARIABLES FIRST
		OE_CORE_ASSERT(m_DirectoriesCreated.pFailedToCreateDirsOrFiles, "pFailedToCreateDirsOrFiles is null!");
		OE_CORE_ASSERT(m_DirectoriesCreated.pFilesCreated, "pFilesCreated is null!");

		std::filesystem::create_directory(projectName);
		m_ProjectPath = projectName + "/" + projectName + ".oeproj";
		
		if (!m_DirectoriesCreated.pFilesCreated->ProjectFileCreated)
		{
			if (!std::filesystem::exists(m_ProjectPath))
			{
				std::ofstream projectFile(m_ProjectPath);
				if (!projectFile)
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile = true;

				ORION_ENGINE_INFO("Successfully created project file: ", m_ProjectPath);
				m_DirectoriesCreated.pFilesCreated->ProjectFileCreated = true;
			}
			else
				m_DirectoriesCreated.pFilesCreated->ProjectFileCreated = true;
		}

		// Create scene file
		m_ScenePath = m_Scene.GetSceneName();
		if (!m_DirectoriesCreated.pFilesCreated->SceneFileCreated)
		{
			if (!std::filesystem::exists(m_ScenePath))
			{
				std::ofstream sceneFile(m_ScenePath);
				if (!sceneFile)
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile = true;
				ORION_ENGINE_INFO("Successfully created scene file: ", m_ScenePath);
				m_DirectoriesCreated.pFilesCreated->SceneFileCreated = true;
			}
			else
				m_DirectoriesCreated.pFilesCreated->SceneFileCreated = true;
		}

		// Create directories first
		// Asset Directory

		if (!m_DirectoriesCreated.AssetDirectoryCreated) // so we dont create the directory more than once
		{
			std::filesystem::path assetDirPath = projectName + "/Assets";
			if (!std::filesystem::exists(assetDirPath))
			{
				if (!std::filesystem::create_directory(assetDirPath))
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory = true;

				ORION_ENGINE_INFO("Successfully created directory: ", assetDirPath);
				m_DirectoriesCreated.AssetDirectoryCreated = true;
			}
			else
				m_DirectoriesCreated.AssetDirectoryCreated = true;
		}

		// Scripts Directory

		if (!m_DirectoriesCreated.ScriptsDirectoryCreated)
		{
			std::filesystem::path scriptsDirPath = projectName + "/Scripts";
			if (!std::filesystem::exists(scriptsDirPath))
			{
				if (std::filesystem::create_directory(scriptsDirPath))
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory = true;

				ORION_ENGINE_INFO("Successfully created directory: ", scriptsDirPath);
				m_DirectoriesCreated.ScriptsDirectoryCreated = true;
			}
			else
				m_DirectoriesCreated.ScriptsDirectoryCreated = true;
		}

		bool ADCreated = m_DirectoriesCreated.AssetDirectoryCreated; // AD = Asset Directory, so AssetDirectoryCreated
		bool SDCreated = m_DirectoriesCreated.ScriptsDirectoryCreated; // SD = Scripts Directory, so ScriptsDirectoryCreated
		bool PFCreated = m_DirectoriesCreated.pFilesCreated->ProjectFileCreated; // PF = Project File, so ProjectFileCreated
		bool SFCreated = m_DirectoriesCreated.pFilesCreated->SceneFileCreated; // SF = Scene File, so SceneFileCreated

		bool allDirectoriesCreated = ADCreated && SDCreated && PFCreated && SFCreated;

		if (allDirectoriesCreated)
		{
			ORION_ENGINE_INFO("All directories created successfully!");
		}

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory)
			ORION_ENGINE_WARNING("Failed to create directory Asset Directory");

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory)
			ORION_ENGINE_WARNING("Failed to create Scripts Directory");

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile)
			ORION_ENGINE_FATAL_ERROR("Failed to create Project File!"); // TODO: Throw an exception later

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile)
			ORION_ENGINE_FATAL_ERROR("Failed to create Scene File!");

		bool everythingFailed = (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile) ? true : false;

		if (everythingFailed)
		{
			ORION_ENGINE_FATAL_ERROR("Internal Engine Error: Failed to create all Asset files and directories");
			std::exit(0);
		}

		return true;
	}

	bool OESceneSerializer::SerializeEntities()
	{
		return true;
	}

	bool OESceneSerializer::SerializeScene()
	{
		return true;
	}
}
