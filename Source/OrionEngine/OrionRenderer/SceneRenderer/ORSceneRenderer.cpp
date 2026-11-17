#include "ORSceneRenderer.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/OrionRenderer/IBL/ORIBLGenerator.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		bool ORSceneRenderer::RenderSceneEnvironment(OEEnvironment& environment)
		{
			OE_CORE_ASSERT(m_Renderer ,"Renderer is null!");

			if (environment.GetEnvironmentHDRTexturePath().empty())
			{
				ORION_ENGINE_INFO("No environment specified. Using default HDR texture.");
				environment.SetEnvironmentHDRTexturePath(R"(D:\dev\OrionEngine\Assets\EngineRendererAssets\EnvironmentTexture\DefaultEnvironmentHDRTexture.hdr)");
			}

			if (!std::filesystem::is_regular_file(environment.GetEnvironmentHDRTexturePath()))
			{
				ORION_ENGINE_ERROR("Invalid file");
				return false;
			}

			std::string extension = environment.GetEnvironmentHDRTexturePath().extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), [](unsigned char character) { return static_cast<char>(std::tolower(character)); });

			if (extension != ".hdr")
			{
				ORION_ENGINE_ERROR("Unsupported extension: ", extension);
				return false;
			}

			AssetManagement::OEAssetIdentifierHandle environmentTextureIdentifierHandle = m_AssetManager->GetAssetIdentifierHandle(environment.GetEnvironmentHDRTexturePath());
			
			if (environmentTextureIdentifierHandle == AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
				environmentTextureIdentifierHandle = m_AssetManager->RegisterAsset(AssetManagement::OEAssetType::Texture, environment.GetEnvironmentHDRTexturePath());

			Ref<AssetManagement::OETexture> enviromentAssetTexture = m_AssetManager->LoadAsset<AssetManagement::OETexture>(environmentTextureIdentifierHandle);

			OE_CORE_ASSERT(enviromentAssetTexture, "Environment asset texturre is null!");

			Ref<ORTexture> environmentTexture = ORTexture::CreateTexture(enviromentAssetTexture->GetTextureData());

			// IBL Generation
			ORIBLGenerator IBLGenerator(*m_Renderer);
			
			ORIBLData IBLData = IBLGenerator.Generate(environmentTexture);

			environment.SetEnvironmentIBLData(std::move(IBLData));
			return true;
		}

		void ORSceneRenderer::Render(ECS::OEECSRegistry& registry, ECS::OEGameEntityID entity)
		{
			auto* transform = registry.TransformComponent.GetComponent(entity);
			auto* physics = registry.PhysicsComponent.GetComponent(entity);
			auto* render = registry.RenderableComponent.GetComponent(entity);

			if (!transform || !render)
				return;

			if (!render->Shader)
				render->Shader = ORDefaultRendererResources::GetDefaultShader();

			if (!render->Mesh)
				render->Mesh = ORDefaultRendererResources::GetDefaultMesh();

			if (!render->Material)
				render->Material = ORDefaultRendererResources::GetDefaultMaterial();

			// Build Model Matrix
			glm::mat4 model = glm::translate(glm::mat4(1.0f), transform->Position)
				* glm::mat4_cast(glm::quat(transform->Rotation))
				* glm::scale(glm::mat4(1.0f), transform->Scale);

			OE_CORE_ASSERT(render->Mesh, "Renderable mesh is null before Submit!");
			m_Renderer->Submit(render->Shader, render->Mesh, render->Material, model);
		}

		void ORSceneRenderer::RenderScene(ECS::OEECSRegistry& registry, const ORCamera& camera, const OEEnvironment& environment, const glm::mat4& viewProjection)
		{
			m_Renderer->BeginScene(viewProjection, camera.GetPosition());

			auto& render = registry.RenderableComponent.GetAllComponents();

			for (auto& [id, comp] : render)
			{
				if (!comp.b_Visible)
					continue;

				Render(registry, id);
			}

			const Ref<ORCubemap>& environmentCubemap = environment.GetEnvironmentIBLData().EnvironmentCubemap;

			if (environmentCubemap)
			{
				glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetView()));
				glm::mat4 skyboxViewProjection = camera.GetProjection() * skyboxView;

				m_Renderer->SubmitSkybox(ORDefaultRendererResources::GetDefaultEnvironmentShader(), ORDefaultRendererResources::GetDefaultMesh(), environmentCubemap, skyboxViewProjection);
			}

			m_Renderer->EndScene();
		}
	}
}