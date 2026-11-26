#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ORGraphicsAPI.h"
#include "ORRenderCommand.h"
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORIndexBuffer.h"
#include "ORVertexBuffer.h"
#include "ORSceneData.h"
#include "ORMesh.h"
#include "ORTexture.h"
#include "ORCubemap.h"
#include "OEDirectionalLight.h"
#include "ORShadowMap.h"

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

            void Init(ORGraphicsAPI API) noexcept;
            void Shutdown() noexcept;
            void SetViewport(int x, int y, int width, int height) noexcept;

            void BeginScene(const glm::mat4& viewProjection, const glm::mat4& lightSpaceMatrix, const glm::vec3& cameraPosition, const ORIBLData& environmentIBLData, const OEDirectionalLight& directionalLight);
            void EndScene() noexcept;

            void Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const Ref<AssetManagement::OEMaterial>& material, const glm::mat4& transform);
            void Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const glm::mat4& transform);
            void Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh);
            void SubmitSkybox(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const Ref<ORCubemap>& cubemap, const glm::mat4& viewProjection);

            glm::mat4 CalculateLightSpaceMatrix(const OEDirectionalLight& directionalLight) noexcept;
            
            void InitShadowMap(uint32_t resolution);
            void BeginShadowPass();
            void SubmitShadow(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const glm::mat4& transform, const glm::mat4& lightSpace);
            void EndShadowPass() noexcept;

            const Ref<ORShadowMap>& GetShadowMap() const noexcept { return m_ShadowMap; }

            void SetGraphicsAPI(ORGraphicsAPI API) noexcept { m_GraphicsAPI = API; }

            static ORGraphicsAPI GetGraphicsAPI() { return m_GraphicsAPI; }

            uint32_t GetShadowMapDepthTextureID() const noexcept { return m_ShadowMap ? m_ShadowMap->GetDepthTextureID() : 0; }

        private:

            OESceneData m_SceneData;

            std::unordered_map<AssetManagement::OEAssetIdentifierHandle, Ref<ORTexture>> m_TextureCache;
            glm::ivec2 m_ViewportSize = { 1280, 720 };

            Ref<ORShadowMap> m_ShadowMap;

            static ORGraphicsAPI m_GraphicsAPI;
        };
    }
}