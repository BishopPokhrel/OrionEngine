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
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

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
		m_ScenePath = m_Scene->GetSceneName() + ".oescene";
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
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		try
		{
			out << YAML::BeginMap;

			for (const auto& entity : m_Scene->GetRegistry().GetAllGameEntities())
			{
				ECS::OEGameEntityID id = entity.ID;

				out << YAML::Key << "Game Entity " + entity.Name;
				out << YAML::Value << YAML::BeginMap;

				// =================== TRANSFORM COMPONENT ================= //

				auto* transform = m_Scene->GetRegistry().GetComponent<ECS::OETransformComponent>(id);

				OE_CORE_ASSERT(transform, "Game Entity is missing its Transform Component!");

				out << YAML::Key << "Transform Component";
				out << YAML::Value << YAML::BeginMap;

				out << YAML::Key << "Position";
				out << YAML::Value << YAML::Flow << YAML::BeginSeq
					<< transform->Position.x
					<< transform->Position.y
					<< transform->Position.z
					<< YAML::EndSeq;

				out << YAML::Key << "Rotation";
				out << YAML::Value << YAML::Flow << YAML::BeginSeq
					<< transform->Rotation.x
					<< transform->Rotation.y
					<< transform->Rotation.z
					<< YAML::EndSeq;

				out << YAML::Key << "Scale";
				out << YAML::Value << YAML::Flow << YAML::BeginSeq
					<< transform->Scale.x
					<< transform->Scale.y
					<< transform->Scale.z
					<< YAML::EndSeq;

				out << YAML::EndMap; // Transform Component

				// =================== PHYSICS COMPONENT ================= //

				if (m_Scene->GetRegistry().HasComponent<ECS::OEPhysicsComponent>(id))
				{
					auto* physics = m_Scene->GetRegistry().GetComponent<ECS::OEPhysicsComponent>(id);

					out << YAML::Key << "Physics Component";
					out << YAML::Value << YAML::BeginMap;

					out << YAML::Key << "Mass";
					out << YAML::Value << physics->Mass;

					out << YAML::Key << "Velocity";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< physics->Velocity.x
						<< physics->Velocity.y
						<< physics->Velocity.z
						<< YAML::EndSeq;

					out << YAML::Key << "Acceleration";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< physics->Accleration.x
						<< physics->Accleration.y
						<< physics->Accleration.z
						<< YAML::EndSeq;

					out << YAML::Key << "Is Static";
					out << YAML::Value << physics->IsStatic;

					out << YAML::Key << "Linear Drag";
					out << YAML::Value << physics->LinearDrag;

					out << YAML::Key << "Radius";
					out << YAML::Value << physics->Radius;

					out << YAML::EndMap; // Physics Component
				}

				// ================= RENDERABLE COMPONENT ================= //

				if (m_Scene->GetRegistry().HasComponent<ECS::OERenderableComponent>(id))
				{
					auto* renderable = m_Scene->GetRegistry().GetComponent<ECS::OERenderableComponent>(id);

					out << YAML::Key << "Renderable Component";
					out << YAML::Value << YAML::BeginMap;

					out << YAML::Key << "Visible";
					out << YAML::Value << renderable->b_Visible;

					out << YAML::EndMap; // Renderable Component
				}

				// =================== CAMERA COMPONENT ================== //

				if (m_Scene->GetRegistry().HasComponent<ECS::OECameraComponent>(id))
				{
					auto* cameraComponent = m_Scene->GetRegistry().GetComponent<ECS::OECameraComponent>(id);

					out << YAML::Key << "Camera Component";
					out << YAML::Value << YAML::BeginMap;

					out << YAML::Key << "Primary";
					out << YAML::Value << cameraComponent->Primary;

					out << YAML::Key << "Near Clip";
					out << YAML::Value << cameraComponent->NearClip;

					out << YAML::Key << "Far Clip";
					out << YAML::Value << cameraComponent->FarClip;

					out << YAML::Key << "Camera";
					out << YAML::Value << YAML::BeginMap;

					const glm::mat4& projectionMatrix = cameraComponent->Camera.GetProjection();
					const glm::mat4& viewMatrix = cameraComponent->Camera.GetView();
					const glm::mat4& viewProjectionMatrix = cameraComponent->Camera.GetViewProjection();

					// Projection Matrix
					out << YAML::Key << "Projection Matrix";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq;

					for (int i = 0; i < 4; i++)
					{
						out << YAML::Flow << YAML::BeginSeq;

						for (int j = 0; j < 4; j++)
						{
							out << projectionMatrix[i][j];
						}

						out << YAML::EndSeq;
					}

					out << YAML::EndSeq;

					// View Matrix
					out << YAML::Key << "View Matrix";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq;

					for (int i = 0; i < 4; i++)
					{
						out << YAML::Flow << YAML::BeginSeq;

						for (int j = 0; j < 4; j++)
						{
							out << viewMatrix[i][j];
						}

						out << YAML::EndSeq;
					}

					out << YAML::EndSeq;

					// View Projection Matrix
					out << YAML::Key << "View Projection Matrix";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq;

					for (int i = 0; i < 4; i++)
					{
						out << YAML::Flow << YAML::BeginSeq;

						for (int j = 0; j < 4; j++)
						{
							out << viewProjectionMatrix[i][j];
						}

						out << YAML::EndSeq;
					}

					out << YAML::EndSeq;

					// Camera Position
					const glm::vec3 cameraPosition = cameraComponent->Camera.GetPosition();

					out << YAML::Key << "Position";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< cameraPosition.x
						<< cameraPosition.y
						<< cameraPosition.z
						<< YAML::EndSeq;

					// Camera Orientation
					out << YAML::Key << "Yaw";
					out << YAML::Value << cameraComponent->Camera.GetYaw();

					out << YAML::Key << "Pitch";
					out << YAML::Value << cameraComponent->Camera.GetPitch();

					// Forward
					const glm::vec3 cameraForward = cameraComponent->Camera.GetForward();

					out << YAML::Key << "Forward";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< cameraForward.x
						<< cameraForward.y
						<< cameraForward.z
						<< YAML::EndSeq;

					// Right
					const glm::vec3 cameraRight = cameraComponent->Camera.GetRight();

					out << YAML::Key << "Right";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< cameraRight.x
						<< cameraRight.y
						<< cameraRight.z
						<< YAML::EndSeq;

					// Up
					const glm::vec3 cameraUp = cameraComponent->Camera.GetUp();

					out << YAML::Key << "Up";
					out << YAML::Value << YAML::Flow << YAML::BeginSeq
						<< cameraUp.x
						<< cameraUp.y
						<< cameraUp.z
						<< YAML::EndSeq;

					out << YAML::EndMap; // Camera data
					out << YAML::EndMap; // Camera Component
				}

				out << YAML::EndMap; // Game Entity
			}

			out << YAML::EndMap; // Entities map

			return true;
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred: ", exception.what());
			return false;
		}
	}

	bool OESceneSerializer::SerializeScene()
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		try
		{
			YAML::Emitter out;

			out << YAML::BeginMap;

			// Scene name
			out << YAML::Key << "Scene";
			out << YAML::Value << m_Scene->GetSceneName();

			// Entities map
			out << YAML::Key << "Entities";
			out << YAML::Value;

			if (!SerializeEntities(out, m_ScenePath))
			{
				ORION_ENGINE_ERROR("Failed to serialize entities!");
				return false;
			}

			out << YAML::EndMap;

			std::ofstream finalSceneFile(m_ScenePath);

			if (!finalSceneFile.is_open())
			{
				ORION_ENGINE_ERROR("Failed to open scene file: ", m_ScenePath.string());
				return false;
			}

			finalSceneFile << out.c_str();

			if (!finalSceneFile.good())
			{
				ORION_ENGINE_ERROR("Failed to write scene data!");
				return false;
			}

			return true;
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred: ", exception.what());
			return false;
		}
	}

	bool OESceneSerializer::DeserializeEntities(const YAMLNode& entityNodes)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		try
		{
			for (auto entity : entityNodes)
			{
				std::string entityKey = entity.first.as<std::string>();
				constexpr std::string_view prefix = "Game Entity ";

				if (!entityKey.starts_with(prefix))
				{
					ORION_ENGINE_ERROR("Invalid entity key: ", entityKey);
					return false;
				}

				std::string entityName = entityKey.substr(prefix.size());
				ECS::OEGameEntityID entityID = m_Scene->GetRegistry().CreateGameEntity(entityName);

				YAMLNode entityNode = entity.second;

				YAMLNode transformNode = entityNode["Transform Component"];
				YAMLNode physicsNode = entityNode["Physics Component"];
				YAMLNode renderNode = entityNode["Renderable Component"];
				YAMLNode cameraNode = entityNode["Camera Component"];

				// =================== TRANSFORM COMPONENT ================= //

				if (transformNode)
				{
					YAMLNode positionNode = transformNode["Position"];
					YAMLNode rotationNode = transformNode["Rotation"];
					YAMLNode scaleNode = transformNode["Scale"];

					glm::vec3 positionTC(
						positionNode[0].as<float>(),
						positionNode[1].as<float>(),
						positionNode[2].as<float>()
					);


					glm::vec3 rotationTC(
						rotationNode[0].as<float>(),
						rotationNode[1].as<float>(),
						rotationNode[2].as<float>()
					);

					glm::vec3 scaleTC(
						scaleNode[0].as<float>(),
						scaleNode[1].as<float>(),
						scaleNode[2].as<float>()
					);

					// Pass the data into the registry
					if (m_Scene->GetRegistry().HasComponent<ECS::OETransformComponent>(entityID))
					{
						auto* transform = m_Scene->GetRegistry().GetComponent<ECS::OETransformComponent>(entityID);

						transform->Position = positionTC;
						transform->Rotation = rotationTC;
						transform->Scale = scaleTC;

					}
				}

				// ============== PHYSICS COMPONENT ============== //

				if (physicsNode)
				{
					YAMLNode massNode = physicsNode["Mass"];
					YAMLNode velocityNode = physicsNode["Velocity"];
					YAMLNode accelerationNode = physicsNode["Acceleration"];
					YAMLNode isStaticNode = physicsNode["Is Static"];
					YAMLNode linearDragNode = physicsNode["Linear Drag"];
					YAMLNode radiusNode = physicsNode["Radius"];

					float massPC = massNode.as<float>();

					glm::vec3 velocityPC(
						velocityNode[0].as<float>(),
						velocityNode[1].as<float>(),
						velocityNode[2].as<float>()
					);

					glm::vec3 accelerationPC(
						accelerationNode[0].as<float>(),
						accelerationNode[1].as<float>(),
						accelerationNode[2].as<float>()
					);

					bool isStaticPC = isStaticNode.as<bool>();
					float linearDragPC = linearDragNode.as<float>();
					float radiusPC = radiusNode.as<float>();

					// Pass the data into the registry
					if (m_Scene->GetRegistry().HasComponent<ECS::OEPhysicsComponent>(entityID))
					{
						auto* physics = m_Scene->GetRegistry().GetComponent<ECS::OEPhysicsComponent>(entityID);

						physics->Mass = massPC;
						physics->Velocity = velocityPC;
						physics->Accleration = accelerationPC;
						physics->IsStatic = isStaticPC;
						physics->LinearDrag = linearDragPC;
						physics->Radius = radiusPC;

					}
				}

				// ============= RENDERABLE COMPONENT ============= //

				if (renderNode)
				{
					YAMLNode visibleNode = renderNode["Visible"];
					bool visibleRC = visibleNode.as<bool>();

					// Pass the data into the registry
					if (m_Scene->GetRegistry().HasComponent<ECS::OERenderableComponent>(entityID))
					{
						auto* render = m_Scene->GetRegistry().GetComponent<ECS::OERenderableComponent>(entityID);
						render->b_Visible = visibleRC;
					}
				}

				// ============ CAMERA COMPONENT =============== //

				if (cameraNode)
				{
					YAMLNode primaryNode = cameraNode["Primary"];
					YAMLNode nearClipNode = cameraNode["Near Clip"];
					YAMLNode farClipNode = cameraNode["Far Clip"];

					bool primaryCC = primaryNode.as<bool>();
					float nearClipCC = nearClipNode.as<float>();
					float farClipCC = farClipNode.as<float>();

					YAMLNode cameraDataNode = cameraNode["Camera"];

					YAMLNode positionNode = cameraDataNode["Position"];
					YAMLNode yawNode = cameraDataNode["Yaw"];
					YAMLNode pitchNode = cameraDataNode["Pitch"];
					YAMLNode forwardNode = cameraDataNode["Forward"];
					YAMLNode rightNode = cameraDataNode["Right"];
					YAMLNode upNode = cameraDataNode["Up"];

					glm::vec3 positionCC(
						positionNode[0].as<float>(),
						positionNode[1].as<float>(),
						positionNode[2].as<float>()
					);

					float yawCC = yawNode.as<float>();
					float pitchCC = pitchNode.as<float>();

					glm::vec3 forwardCC(
						forwardNode[0].as<float>(),
						forwardNode[1].as<float>(),
						forwardNode[2].as<float>()
					);

					glm::vec3 rightCC(
						rightNode[0].as<float>(),
						rightNode[1].as<float>(),
						rightNode[2].as<float>()
					);

					glm::vec3 upCC(
						upNode[0].as<float>(),
						upNode[1].as<float>(),
						upNode[2].as<float>()
					);

					// Pass the data into the registry
					if (m_Scene->GetRegistry().HasComponent<ECS::OECameraComponent>(entityID))
					{
						auto* camera = m_Scene->GetRegistry().GetComponent<ECS::OECameraComponent>(entityID);

						camera->FarClip = farClipCC;
						camera->NearClip = nearClipCC;
						camera->Primary = primaryCC;

						// Restore the internal camera state
						camera->Camera.SetPosition(positionCC);
						camera->Camera.Rotate(
							yawCC - camera->Camera.GetYaw(),
							pitchCC - camera->Camera.GetPitch()
						);
					}
				}
			}

			return true;
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred:", exception.what());
			return false;
		}

	}

	bool OESceneSerializer::DeserializeScene(const std::filesystem::path& scenePath)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		try
		{
			YAMLNode data = YAML::LoadFile(scenePath.string());

			if (!data || !data.IsMap())
			{
				ORION_ENGINE_ERROR("Failed to deserialize scene: Invalid root");
				return false;
			}

			YAMLNode sceneNode = data["Scene"];

			if (!sceneNode || !sceneNode.IsScalar())
			{
				ORION_ENGINE_ERROR("Invalid scene node: Missing or invalid!");
				return false; 
			}

			std::string sceneName = sceneNode.as<std::string>();

			ORION_ENGINE_INFO("Deserializing scene: ", sceneName);

			YAMLNode entitiesNode = data["Entities"];

			if (!entitiesNode || !entitiesNode.IsMap())
			{
				ORION_ENGINE_ERROR("Invalid or missing Entities map!");
				return false;
			}

			if (!DeserializeEntities(entitiesNode))
			{
				ORION_ENGINE_ERROR("Failed to deserialize entities");
				return false;
			}

			ORION_ENGINE_INFO("Successfully deserialized ", sceneName);
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred: ", exception.what());
			return false; 
		}

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
