#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>

#include <glm/glm.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"
#include "ORSceneData.h"
#include "ORMesh.h"
#include "ORTexture.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OELogging.h>

#include <OrionEngine/Core/AssetManagement/OEAssetIdentifierHandle.h>
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
                m_TextureCache.clear();
            }

            void SetViewport(int x, int y, int width, int height) noexcept
            {
                ORRenderCommand::SetViewport(x, y, width, height);
            }

            void BeginScene(const glm::mat4& viewProjection, const glm::vec3& cameraPosition) noexcept
            {
                m_SceneData.ViewProjectionMatrix = viewProjection;
                m_SceneData.CameraPosition = cameraPosition;
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
                shader->SetVec3("u_CameraPosition", m_SceneData.CameraPosition);

                const Ref<AssetManagement::OETexture>& albedoTexture = material->GetMaterialAlbedoTexture();
                bool hasAlbedoTexture = false;

                if (albedoTexture)
                {
                    const AssetManagement::OETextureData& textureData = albedoTexture->GetTextureData();
                    const AssetManagement::OEAssetIdentifierHandle identifierHandle = textureData.TextureAssetMetadata.IdentifierHandle;

                    if (identifierHandle != AssetManagement::INVALID_ASSET_IDENTIFIER_HANDLE)
                    {
                        auto textureIterator = m_TextureCache.find(identifierHandle);

                        if (textureIterator == m_TextureCache.end())
                        {
                            Ref<ORTexture> rendererTexture = ORTexture::CreateTexture(textureData);

                            if (rendererTexture)
                                textureIterator = m_TextureCache.emplace(identifierHandle, std::move(rendererTexture)).first;
                        }

                        if (textureIterator != m_TextureCache.end())
                        {
                            textureIterator->second->Bind(0);
                            hasAlbedoTexture = true;
                        }
                    }
                    else
                        ORION_ENGINE_WARNING("Albedo texture has an invalid asset identifier!");
                }

                shader->SetInt("u_AlbedoTexture", 0);
                shader->SetInt("u_HasAlbedoTexture", hasAlbedoTexture ? 1 : 0);

                mesh->Bind();
                ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
            }

            void SetGraphicsAPI(ORGraphicsAPI API) noexcept
            {
                m_GraphicsAPI = API;
            }

            static ORGraphicsAPI GetGraphicsAPI() { return m_GraphicsAPI; }

        private:

            OESceneData m_SceneData;

            std::unordered_map<AssetManagement::OEAssetIdentifierHandle, Ref<ORTexture>> m_TextureCache;

            static ORGraphicsAPI m_GraphicsAPI;
        };
    }
}