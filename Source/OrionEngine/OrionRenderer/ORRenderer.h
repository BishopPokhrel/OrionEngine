#pragma once

#include <glm/glm.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"
#include "ORSceneData.h"
#include "ORMesh.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OELogging.h>

#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OEMaterial.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderer
		{
		public:

			void Init(ORGraphicsAPI API) noexcept
			{
				SetGraphicsAPI(API);
				ORRenderCommand::ORInit();
			}

			void Shutdown() noexcept
			{

			}

			void SetViewport(int x, int y, int width, int height) noexcept
			{
				ORRenderCommand::SetViewport(x, y, width, height);
			}

			void BeginScene(const glm::mat4& viewProjection, const glm::vec3& cameraForward) noexcept
			{
				m_SceneData.ViewProjectionMatrix = viewProjection;
				m_SceneData.CameraForward = cameraForward;
			}

			void EndScene() noexcept
			{

			}

			void Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const Ref<AssetManagement::OEMaterial>& material, const glm::mat4& transform)
			{
				OE_CORE_ASSERT(shader, "Submit received a null shader!");
				OE_CORE_ASSERT(mesh, "Submit received a null mesh!");
				OE_CORE_ASSERT(material, "Submit received a null material!");

				const AssetManagement::OEMaterialData& materialData = material->GetMaterialData();

				shader->Bind();
				shader->SetMat4("u_ViewProjection", m_SceneData.ViewProjectionMatrix);
				shader->SetMat4("u_Transform", transform);
				shader->SetVec3("u_LightDirection", glm::vec3(-0.5f, -1.0f, -0.5f));
				shader->SetVec3("u_LightColor", glm::vec3(1.0f));
				shader->SetFloat("u_LightIntensity", 1.0f);

				shader->SetVec4("u_AlbedoColor", materialData.AlbedoColor);
				shader->SetFloat("u_Metallic", materialData.Metallic);
				shader->SetFloat("u_Roughness", materialData.Roughness);
				shader->SetFloat("u_AmbientOcclusion", materialData.AmbientOcclusion);

				shader->SetVec3("u_CameraForward", m_SceneData.CameraForward);

				mesh->Bind();

				ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
			}

			void SetGraphicsAPI(ORGraphicsAPI API) noexcept
			{
				m_GraphicsAPI = API;
			}

			ORGraphicsAPI GetGraphicsAPI() const noexcept { return m_GraphicsAPI; }

		private:
		
			SceneData m_SceneData;
			ORGraphicsAPI m_GraphicsAPI;
		};
	}
}