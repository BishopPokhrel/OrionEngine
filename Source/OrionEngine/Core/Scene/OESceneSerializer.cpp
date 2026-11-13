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

		try
		{
			if (projectName.empty() || m_Scene->GetSceneName().empty())
				return false;

			// Resolve all paths to absolute paths once.
			const std::filesystem::path projectDirectory = std::filesystem::absolute(projectName).lexically_normal();

			const std::string projectFileName = projectDirectory.filename().string();

			if (projectFileName.empty())
				return false;

			m_ProjectPath = projectDirectory / (projectFileName + ".oeproj");
			m_ScenePath = projectDirectory / (m_Scene->GetSceneName() + ".oescene");

			const std::filesystem::path assetDirectory = projectDirectory / "Assets";
			const std::filesystem::path scriptsDirectory = projectDirectory / "Scripts";

			// Reset creation status.
			m_DirectoriesCreated.ProjectDirectoryCreated = false;
			m_DirectoriesCreated.AssetDirectoryCreated = false;
			m_DirectoriesCreated.ScriptsDirectoryCreated = false;

			m_DirectoriesCreated.pFilesCreated->ProjectFileCreated = false;
			m_DirectoriesCreated.pFilesCreated->SceneFileCreated = false;

			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory = false;
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile = false;
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile = false;
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory = false;
			m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory = false;

			// Create project directory.
			if (!std::filesystem::exists(projectDirectory))
			{
				if (!std::filesystem::create_directories(projectDirectory))
				{
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory = true;
					ORION_ENGINE_ERROR("Failed to create project directory: ", projectDirectory.string());
					return false;
				}
			}
			else if (!std::filesystem::is_directory(projectDirectory))
			{
				m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectDirectory = true;
				ORION_ENGINE_ERROR("Project path exists but is not a directory: ", projectDirectory.string());
				return false;
			}

			m_DirectoriesCreated.ProjectDirectoryCreated = true;

			// Do not overwrite an existing project file.
			if (std::filesystem::exists(m_ProjectPath))
			{
				ORION_ENGINE_WARNING("Project already exists: ", m_ProjectPath.string());
				return false;
			}

			// Create project file.
			{
				std::ofstream projectFile(m_ProjectPath);

				if (!projectFile.is_open() || !projectFile.good())
				{
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateProjectFile = true;
					ORION_ENGINE_ERROR("Failed to create project file: ", m_ProjectPath.string());
					return false;
				}
			}

			m_DirectoriesCreated.pFilesCreated->ProjectFileCreated = true;
			ORION_ENGINE_INFO("Successfully created project file: ", m_ProjectPath.string());

			// Create scene file.
			{
				std::ofstream sceneFile(m_ScenePath);

				if (!sceneFile.is_open() || !sceneFile.good())
				{
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateSceneFile = true;
					ORION_ENGINE_ERROR("Failed to create scene file: ", m_ScenePath.string());
					return false;
				}
			}

			m_DirectoriesCreated.pFilesCreated->SceneFileCreated = true;
			ORION_ENGINE_INFO("Successfully created scene file: ", m_ScenePath.string());

			// Create Assets directory.
			if (!std::filesystem::exists(assetDirectory))
			{
				if (!std::filesystem::create_directory(assetDirectory))
				{
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory = true;
					ORION_ENGINE_ERROR("Failed to create Assets directory: ", assetDirectory.string());
					return false;
				}
			}
			else if (!std::filesystem::is_directory(assetDirectory))
			{
				m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateAssetsDirectory = true;
				ORION_ENGINE_ERROR("Assets path exists but is not a directory: ", assetDirectory.string());
				return false;
			}

			m_DirectoriesCreated.AssetDirectoryCreated = true;
			ORION_ENGINE_INFO("Assets directory is ready: ", assetDirectory.string());

			// Create Scripts directory.
			if (!std::filesystem::exists(scriptsDirectory))
			{
				if (!std::filesystem::create_directory(scriptsDirectory))
				{
					m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory = true;
					ORION_ENGINE_ERROR("Failed to create Scripts directory: ", scriptsDirectory.string());
					return false;
				}
			}
			else if (!std::filesystem::is_directory(scriptsDirectory))
			{
				m_DirectoriesCreated.pFailedToCreateDirsOrFiles->FailedToCreateScriptsDirectory = true;
				ORION_ENGINE_ERROR("Scripts path exists but is not a directory: ", scriptsDirectory.string());
				return false;
			}

			m_DirectoriesCreated.ScriptsDirectoryCreated = true;
			ORION_ENGINE_INFO("Scripts directory is ready: ", scriptsDirectory.string());

			ORION_ENGINE_INFO("All project directories and files created successfully!");
			ORION_ENGINE_INFO("Project path: ", m_ProjectPath.string());
			ORION_ENGINE_INFO("Scene path: ", m_ScenePath.string());

			return true;
		}
		catch (const std::filesystem::filesystem_error& err)
		{
			ORION_ENGINE_ERROR("Filesystem error: ", err.code().message());
			return false;
		}
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR(e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
			return false;
		}
	}

	bool OESceneSerializer::OpenOEProjectDirectory(const std::filesystem::path& projectPath)
	{
		try
		{
			if (projectPath.empty() || projectPath.extension() != ".oeproj" || !std::filesystem::exists(projectPath) || !std::filesystem::is_regular_file(projectPath))
				return false;

			const std::filesystem::path projectPathDirectory = projectPath.parent_path();

			if (!std::filesystem::is_directory(projectPathDirectory / "Assets") || !std::filesystem::is_directory(projectPathDirectory / "Scripts"))
				return false;

			m_ProjectPath = std::filesystem::absolute(projectPath).lexically_normal();
			return true;
		}
		catch (const std::filesystem::filesystem_error& err)
		{
			ORION_ENGINE_ERROR(err.code().message());
			return false;
		}
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR(e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
			return false;
		}
	}

	bool OESceneSerializer::SerializeEntities(YAML::Emitter& out, const std::filesystem::path& path)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");
		OE_CORE_ASSERT(m_AssetManager, "Asset Manager is null!");

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

					if (renderable->MeshIdentifierHandle != AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
					{
						out << YAML::Key << "Mesh";
						out << YAML::Value << YAML::BeginMap;

						out << YAML::Key << "Path";
						out << YAML::Value << std::filesystem::relative(m_AssetManager->GetAssetPath(renderable->MeshIdentifierHandle), m_ProjectPath.parent_path()).string();

						out << YAML::EndMap;
					}

					if (renderable->MaterialIdentifierHandle != AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
					{
						out << YAML::Key << "Material";
						out << YAML::Value << YAML::BeginMap;

						out << YAML::Key << "Path";
						out << YAML::Value << std::filesystem::relative(m_AssetManager->GetAssetPath(renderable->MaterialIdentifierHandle), m_ProjectPath.parent_path()).string();

						out << YAML::Key << "Material Properties";
						out << YAML::Value << YAML::BeginMap;

						// Albedo, Metallic, Roughness & Ambient Occlusion
						const auto& materialData = renderable->Material->GetMaterialData();
						Ref<AssetManagement::OETexture> materialAlbedoTexture = renderable->Material->GetMaterialAlbedoTexture();

						out << YAML::Key << "Albedo Color";
						out << YAML::Value << YAML::Flow << YAML::BeginSeq
							<< materialData.AlbedoColor.x
							<< materialData.AlbedoColor.y
							<< materialData.AlbedoColor.z
							<< materialData.AlbedoColor.a << YAML::EndSeq;

						out << YAML::Key << "Metallic";
						out << YAML::Value << materialData.Metallic;

						out << YAML::Key << "Roughness";
						out << YAML::Value << materialData.Roughness;

						out << YAML::Key << "Ambient Occlusion";
						out << YAML::Value << materialData.AmbientOcclusion;

						if (materialAlbedoTexture)
						{
							out << YAML::Key << "Texture Properties";
							out << YAML::Value << YAML::BeginMap;

							out << YAML::Key << "Texture Path";
							out << YAML::Value << (m_ProjectPath.parent_path() / materialAlbedoTexture->GetTextureData().TextureAssetMetadata.Path).string();

							out << YAML::EndMap;
						}

						out << YAML::EndMap;

						out << YAML::EndMap;
					}

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
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR("Exception: ", e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
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

			ORION_ENGINE_INFO("Attempting to save scene at ", m_ScenePath.string());
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
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR("Exception: ", e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
			return false;
		}
	}

	bool OESceneSerializer::DeserializeEntities(const YAMLNode& entityNodes)
	{
		OE_CORE_ASSERT(m_Scene, "Scene is null!");

		try
		{
			auto& registry = m_Scene->GetRegistry();

			for (const auto& entity : entityNodes)
			{
				std::string entityKey = entity.first.as<std::string>();

				if (entityKey == "Scene")
					continue;

				constexpr std::string_view prefix = "Game Entity ";

				if (!entityKey.starts_with(prefix))
				{
					ORION_ENGINE_ERROR("Invalid entity key: ", entityKey);
					return false;
				}

				std::string entityName = entityKey.substr(prefix.size());

				ECS::OEGameEntityID entityID = registry.CreateGameEntity(entityName);
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

					ECS::OETransformComponent transformComponent{};
					transformComponent.Position = positionTC;
					transformComponent.Rotation = rotationTC;
					transformComponent.Scale = scaleTC;

					registry.TransformComponent.AddComponent(entityID, transformComponent);
				}

				// =================== PHYSICS COMPONENT ==================== //

				if (physicsNode)
				{
					YAMLNode velocityNode = physicsNode["Velocity"];
					YAMLNode accelerationNode = physicsNode["Acceleration"];

					ECS::OEPhysicsComponent physicsComponent{};

					physicsComponent.Mass = physicsNode["Mass"].as<float>();

					physicsComponent.Velocity = glm::vec3(
						velocityNode[0].as<float>(),
						velocityNode[1].as<float>(),
						velocityNode[2].as<float>()
					);

					physicsComponent.Accleration = glm::vec3(
						accelerationNode[0].as<float>(),
						accelerationNode[1].as<float>(),
						accelerationNode[2].as<float>()
					);

					physicsComponent.IsStatic = physicsNode["Is Static"].as<bool>();
					physicsComponent.LinearDrag = physicsNode["Linear Drag"].as<float>();
					physicsComponent.Radius = physicsNode["Radius"].as<float>();

					registry.PhysicsComponent.AddComponent(entityID, physicsComponent);
				}

				// ================== RENDERABLE COMPONENT ================== //

				if (renderNode)
				{
					ECS::OERenderableComponent renderComponent{};

					renderComponent.b_Visible = renderNode["Visible"].as<bool>();

					YAMLNode meshNode = renderNode["Mesh"];

					if (meshNode)
					{
						std::filesystem::path assetMeshPath = m_ProjectPath.parent_path() / meshNode["Path"].as<std::string>();
						assetMeshPath = std::filesystem::absolute(assetMeshPath).lexically_normal();

						AssetManagement::OEAssetIdentifierHandle assetMeshIdentifierHandle = m_AssetManager->GetAssetIdentifierHandle(assetMeshPath);

						if (assetMeshIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
							assetMeshIdentifierHandle = m_AssetManager->RegisterAsset(AssetManagement::OEAssetType::Mesh, assetMeshPath);

						if (assetMeshIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
						{
							ORION_ENGINE_ERROR("Failed to register Mesh asset: ", assetMeshPath.string());
							return false;
						}

						Ref<AssetManagement::OEMesh> assetMesh = m_AssetManager->LoadAsset<AssetManagement::OEMesh>(assetMeshIdentifierHandle);

						if (!assetMesh)
						{
							ORION_ENGINE_ERROR("Failed to load Mesh asset: ", assetMeshPath.string());
							return false;
						}

						renderComponent.MeshIdentifierHandle = assetMeshIdentifierHandle;
						renderComponent.Mesh = assetMesh->GetMesh();
					}

					YAMLNode materialNode = renderNode["Material"];

					if (materialNode)
					{
						std::filesystem::path assetMaterialPath = m_ProjectPath.parent_path() / materialNode["Path"].as<std::string>();
						assetMaterialPath = std::filesystem::absolute(assetMaterialPath).lexically_normal();

						AssetManagement::OEAssetIdentifierHandle assetMaterialIdentifierHandle = m_AssetManager->GetAssetIdentifierHandle(assetMaterialPath);

						if (assetMaterialIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
							assetMaterialIdentifierHandle = m_AssetManager->RegisterAsset(AssetManagement::OEAssetType::Material, assetMaterialPath);

						if (assetMaterialIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
						{
							ORION_ENGINE_ERROR("Failed to register Material asset: ", assetMaterialPath.string());
							return false;
						}

						Ref<AssetManagement::OEMaterial> assetMaterial = m_AssetManager->LoadAsset<AssetManagement::OEMaterial>(assetMaterialIdentifierHandle);

						if (!assetMaterial)
						{
							ORION_ENGINE_ERROR("Failed to load Material asset: ", assetMaterialPath.string());
							return false;
						}

						renderComponent.MaterialIdentifierHandle = assetMaterialIdentifierHandle;
						renderComponent.Material = assetMaterial;

						AssetManagement::OEAssetMetadata materialMetadata;
						materialMetadata.IdentifierHandle = assetMaterialIdentifierHandle;
						materialMetadata.Type = AssetManagement::OEAssetType::Material;
						materialMetadata.Path = assetMaterialPath;

						assetMaterial->SetMaterialAssetMetadata(materialMetadata);

						// Deserialize the actual material properities after ensuring that the material is loaded correctly
											
						YAMLNode materialProperitesNode = materialNode["Material Properties"];

						if (materialProperitesNode)
						{
							YAMLNode materialAlbedoColorNode = materialProperitesNode["Albedo Color"];
							YAMLNode materialMetallicNode = materialProperitesNode["Metallic"];
							YAMLNode materialRoughnessNode = materialProperitesNode["Roughness"];
							YAMLNode materialAmbientOcclusionNode = materialProperitesNode["Ambient Occlusion"];

							YAMLNode texturePropertiesNode = materialProperitesNode["Texture Properties"];

							glm::vec4 albedoColorMaterial = (materialAlbedoColorNode && materialAlbedoColorNode.IsSequence() && materialAlbedoColorNode.size() == 4) ? glm::vec4(
								materialAlbedoColorNode[0].as<float>(),
								materialAlbedoColorNode[1].as<float>(),
								materialAlbedoColorNode[2].as<float>(),
								materialAlbedoColorNode[3].as<float>()
							) : assetMaterial->GetMaterialData().AlbedoColor;

							float metallicMaterial = materialMetallicNode ? materialMetallicNode.as<float>() : assetMaterial->GetMaterialData().Metallic;
							float roughnessMaterial = materialRoughnessNode ? materialRoughnessNode.as<float>() : assetMaterial->GetMaterialData().Roughness;
							float ambientOcclusionMaterial = materialAmbientOcclusionNode ? materialAmbientOcclusionNode.as<float>() : assetMaterial->GetMaterialData().AmbientOcclusion;

							if (texturePropertiesNode)
							{
								std::filesystem::path textureMaterialPath = m_ProjectPath.parent_path() / texturePropertiesNode["Texture Path"].as<std::string>();
								textureMaterialPath = std::filesystem::absolute(textureMaterialPath).lexically_normal();

								AssetManagement::OEAssetIdentifierHandle textureIdentifierHandle = m_AssetManager->GetAssetIdentifierHandle(textureMaterialPath);

								if (textureIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
									textureIdentifierHandle = m_AssetManager->RegisterAsset(AssetManagement::OEAssetType::Texture, textureMaterialPath);

								if (textureIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
								{
									ORION_ENGINE_ERROR("Failed to register texture asset at path ", textureMaterialPath.string());
									return false;
								}

								Ref<AssetManagement::OETexture> texture = m_AssetManager->LoadAsset<AssetManagement::OETexture>(textureIdentifierHandle);

								if (!texture)
								{
									ORION_ENGINE_ERROR("Failed to load asset at path ", textureMaterialPath.string());
									return false;
								}

								assetMaterial->SetAlbedoTexture(texture);
							}

							renderComponent.Material->SetAlbedoColor(albedoColorMaterial);
							renderComponent.Material->SetMetallic(metallicMaterial);
							renderComponent.Material->SetRoughness(roughnessMaterial);
							renderComponent.Material->SetAmbientOcclusion(ambientOcclusionMaterial);
						}
					}

					registry.RenderableComponent.AddComponent(entityID, renderComponent);
				}

				// ==================== CAMERA COMPONENT ==================== //

				if (cameraNode)
				{
					ECS::OECameraComponent cameraComponent{};

					cameraComponent.Primary = cameraNode["Primary"].as<bool>();
					cameraComponent.NearClip = cameraNode["Near Clip"].as<float>();
					cameraComponent.FarClip = cameraNode["Far Clip"].as<float>();

					YAMLNode cameraDataNode = cameraNode["Camera"];

					glm::vec3 positionCC(
						cameraDataNode["Position"][0].as<float>(),
						cameraDataNode["Position"][1].as<float>(),
						cameraDataNode["Position"][2].as<float>()
					);

					float yawCC = cameraDataNode["Yaw"].as<float>();
					float pitchCC = cameraDataNode["Pitch"].as<float>();

					cameraComponent.Camera.SetPosition(positionCC);
					cameraComponent.Camera.Rotate(
						yawCC - cameraComponent.Camera.GetYaw(),
						pitchCC - cameraComponent.Camera.GetPitch()
					);

					registry.CameraComponent.AddComponent(entityID, cameraComponent);
				}
			}

			return true;
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred: ", exception.what());
			return false;
		}
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR("Exception: ", e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
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

			m_ScenePath = scenePath;
			m_Scene->SetSceneName(sceneName);

			ORION_ENGINE_INFO("Successfully deserialized ", sceneName);
		}
		catch (const YAML::Exception& exception)
		{
			ORION_ENGINE_ERROR("YAML Exception occurred: ", exception.what());
			return false; 
		}
		catch (const std::exception& e)
		{
			ORION_ENGINE_ERROR("Exception: ", e.what());
			return false;
		}
		catch (...)
		{
			ORION_ENGINE_ERROR("Unknown exception found!");
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
