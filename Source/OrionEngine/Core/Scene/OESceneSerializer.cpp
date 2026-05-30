#include "OESceneSerializer.h"

#include <filesystem>
#include <fstream>

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <yaml-cpp/yaml.h>

namespace OrionEngine
{
	bool OESceneSerializer::CreateOEProjectDirectory(const std::string& projectName)
	{		
		// ASSERT POINTER VARIABLES FIRST
		OE_CORE_ASSERT(m_DirectoriesCreated.pFailedToCreateDirsOrFiles, "pFailedToCreateDirsOrFiles is null!");
		OE_CORE_ASSERT(m_DirectoriesCreated.pFilesCreated, "pFilesCreated is null!");

		if (!std::filesystem::create_directory(projectName))
		{
			if (DoesProjectExist(projectName))
				return false;

			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory = true;
		}
		else
		{
			m_ProjectPath = projectName + "/" + projectName + ".oeproj";
			m_DirectoriesCreated.ProjectDirectoryCreated = true;
		}
		
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
		m_ScenePath = m_Scene.GetSceneName() + ".oescene";
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
				if (!std::filesystem::create_directory(scriptsDirPath))
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory = true;

				ORION_ENGINE_INFO("Successfully created directory: ", scriptsDirPath);
				m_DirectoriesCreated.ScriptsDirectoryCreated = true;
			}
			else
				m_DirectoriesCreated.ScriptsDirectoryCreated = true;
		}

		bool ADCreated = m_DirectoriesCreated.AssetDirectoryCreated; // AD = Asset Directory, so AssetDirectoryCreated
		bool SDCreated = m_DirectoriesCreated.ScriptsDirectoryCreated; // SD = Scripts Directory, so ScriptsDirectoryCreated
		bool PDCreated = m_DirectoriesCreated.ProjectDirectoryCreated;
		bool PFCreated = m_DirectoriesCreated.pFilesCreated->ProjectFileCreated; // PF = Project File, so ProjectFileCreated
		bool SFCreated = m_DirectoriesCreated.pFilesCreated->SceneFileCreated; // SF = Scene File, so SceneFileCreated

		bool allDirectoriesCreatedAndFiles = ADCreated && SDCreated && PFCreated && SFCreated && PDCreated;

		if (allDirectoriesCreatedAndFiles)
		{
			ORION_ENGINE_INFO("All directories and files created successfully!");
		}

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory)
			ORION_ENGINE_WARNING("Failed to create directory Asset Directory");

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory)
			ORION_ENGINE_WARNING("Failed to create Scripts Directory");

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory)
			ORION_ENGINE_FATAL_ERROR("Failed to create Project: ", projectName, " directory!");

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile)
			ORION_ENGINE_FATAL_ERROR("Failed to create Project File!"); // TODO: Throw an exception later

		if (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile)
			ORION_ENGINE_FATAL_ERROR("Failed to create Scene File!");

		bool everythingFailed = (m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile && 
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile &&
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory) ? true : false;

		if (everythingFailed)
		{
			ORION_ENGINE_FATAL_ERROR("Internal Engine Error: Failed to create all Asset files and directories");
			std::exit(0);
		}

		return true;
	}

	bool OESceneSerializer::SerializeEntities(YAML::Emitter& out, const std::filesystem::path& path)
	{
		out << YAML::BeginMap;

		for (const auto& entity : m_Scene.GetRegistry().GetAllGameEntities())
		{
			ECS::OEGameEntityID id = entity.ID;
			out << YAML::Key << "Game Entity " + entity.Name;
			out << YAML::Value;

			// Transform Component
			out << YAML::BeginMap;

			out << YAML::Key << "Transform Component";
			out << YAML::Value;

			// Position:
			out << YAML::Key << "Position";
			out << YAML::Value;
			out << YAML::Flow << YAML::BeginSeq
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Position.x
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Position.y
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Position.z
				<< YAML::EndSeq;

			// Rotation:
			out << YAML::Key << "Rotation";
			out << YAML::Value;
			out << YAML::Flow << YAML::BeginSeq
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Rotation.x
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Rotation.y
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Rotation.z
				<< YAML::EndSeq;

			out << YAML::Key << "Scale";
			out << YAML::Value;
			out << YAML::Flow << YAML::BeginSeq
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Scale.x
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Scale.y
				<< m_Scene.GetRegistry().GetComponent<ECS::OETransformComponent>(id)->Scale.z
				<< YAML::EndSeq;

			out << YAML::EndMap;

			// Physics Component
			if (m_Scene.GetRegistry().HasComponent<ECS::OEPhysicsComponent>(entity.ID))
			{
				out << YAML::Key << "Physics Component";
				out << YAML::Value;
				out << YAML::BeginMap;
				
				// Mass
				out << YAML::Key << "Mass";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Mass;

				// Velocity
				out << YAML::Key << "Velocity";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Velocity.x
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Velocity.y
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Velocity.z
					<< YAML::EndSeq;

				// Accleration
				out << YAML::Key << "Accleration";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Accleration.x
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Accleration.y
					<< m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Accleration.z
					<< YAML::EndSeq;

				// Is Static
				out << YAML::Key << "Is Static";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->IsStatic;

				out << YAML::Key << "Linear Drag";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->LinearDrag;

				out << YAML::Key << "Radius";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entity.ID)->Radius;

				out << YAML::EndMap;
			}

			// Renderable Component
			if (m_Scene.GetRegistry().GetComponent<ECS::OERenderableComponent>(entity.ID))
			{
				out << YAML::Key << "Renderable Component";
				out << YAML::Value;
				out << YAML::BeginMap;

				// Visible
				out << YAML::Key << "Visible";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OERenderableComponent>(entity.ID)->b_Visible;

				out << YAML::EndMap;
			}

			// Camera Component
			if (m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID))
			{
				out << YAML::Key << "Camera Component";
				out << YAML::Value;
				out << YAML::BeginMap;

				// Primary
				out << YAML::Key << "Primary";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Primary;

				// Near Clip
				out << YAML::Key << "Near Clip";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->NearClip;

				// Far Clip
				out << YAML::Key << "Far Clip";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->FarClip;

				// ACTUAL CAMERA PROPERTIES
				out << YAML::Key << "Camera";
				out << YAML::Value;
				out << YAML::BeginMap;

				const glm::mat4& projectionMatrix = m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetProjection();
				const glm::mat4& viewMatrix = m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetView();
				const glm::mat4& viewProjectionMatrix = m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetViewProjection();

				// PROJECTION MATRIX
				out << YAML::Key << "Projection Matrix";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq;

				for (int i = 0; i < 4; i++)
				{
					out << YAML::Flow << YAML::BeginSeq;
					for (int j = 0; j < 4; j++)
					{
						out << projectionMatrix[i][j];
					}
					out << YAML::EndSeq;
				}

				out << YAML::EndSeq; // END PROJECTION MATRIX SEQ

				// VIEW MATRIX
				out << YAML::Key << "View Matrix";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq; 

				for (int i = 0; i < 4; i++)
				{
					out << YAML::Flow << YAML::BeginSeq;
					for (int j = 0; j < 4; j++)
					{
						out << viewMatrix[i][j];
					}
					out << YAML::EndSeq;
				}

				out << YAML::EndSeq; // END VIEW MATRIX SEQ

				// VIEW PROJECTION MATRIX
				out << YAML::Key << "View Projection Matrix";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq;

				for (int i = 0; i < 4; i++)
				{
					out << YAML::Flow << YAML::BeginSeq;
					for (int j = 0; j < 4; j++)
					{
						out << viewProjectionMatrix[i][j];
					}
					out << YAML::EndSeq;
				}

				out << YAML::EndSeq; // END VIEW PROJECTION MATRIX SEQ

				// POSITION 
				out << YAML::Key << "Position";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetPosition().x
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetPosition().y
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetPosition().z
					<< YAML::EndSeq;

				// YAW
				out << YAML::Key << "Yaw";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetYaw();

				// PITCH
				out << YAML::Key << "Pitch";
				out << YAML::Value << m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetPitch();

				// FORWARD
				out << YAML::Key << "Forward";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetForward().x
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetForward().y
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetForward().z
					<< YAML::EndSeq;

				// RIGHT
				out << YAML::Key << "Right";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetRight().x
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetRight().y
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetRight().z
					<< YAML::EndSeq;

				// UP
				out << YAML::Key << "Up";
				out << YAML::Value;
				out << YAML::Flow << YAML::BeginSeq
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetUp().x
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetUp().y
					<< m_Scene.GetRegistry().GetComponent<ECS::OECameraComponent>(entity.ID)->Camera.GetUp().z
					<< YAML::EndSeq;


				out << YAML::EndMap; // CAMERA MAP END

				out << YAML::EndMap; // CAMERA COMPONENT MAP END
			}
		}

		out << YAML::EndMap;

		return true;
	}

	bool OESceneSerializer::SerializeScene()
	{
		YAML::Emitter out;
		
		out << YAML::BeginMap;

		out << YAML::Key << "Scene";
		out << YAML::Value << m_Scene.GetSceneName();

		SerializeEntities(out, m_ScenePath);

		out << YAML::EndMap;

		std::ofstream finalSceneFile(m_ScenePath);
		finalSceneFile << out.c_str();

		return true;
	}

	bool OESceneSerializer::DoesProjectExist(const std::string& projectName)
	{
		std::filesystem::path projectFilePath = m_ProjectPath;
		std::filesystem::path sceneFilePath = m_ScenePath;
		std::filesystem::path assetDirPath = projectName + "/Assets";
		std::filesystem::path scriptsDirPath = projectName + "/Scripts";
		std::filesystem::path projectDirPath = projectName;

		return (
			std::filesystem::exists(projectFilePath) &&
			std::filesystem::exists(sceneFilePath) &&
			std::filesystem::exists(assetDirPath) &&
			std::filesystem::exists(scriptsDirPath) &&
			std::filesystem::exists(projectDirPath)
			) ? true : false;
	}
}
