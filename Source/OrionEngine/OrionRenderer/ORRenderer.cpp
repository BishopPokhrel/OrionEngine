#include "ORRenderer.h"

namespace OrionEngine::OrionRenderer
{
	ORGraphicsAPI ORRenderer::m_GraphicsAPI = ORGraphicsAPI::None;

	void ORRenderer::Init(ORGraphicsAPI API) noexcept
	{
		SetGraphicsAPI(API);
		ORRenderCommand::ORInit();
	}

	void ORRenderer::Shutdown() noexcept
	{
		m_TextureCache.clear();
	}

	void ORRenderer::SetViewport(int x, int y, int width, int height) noexcept
	{
        m_ViewportSize = { width, height };
		ORRenderCommand::SetViewport(x, y, width, height);
	}

	void ORRenderer::BeginScene(const glm::mat4& viewProjection, const glm::mat4& lightSpaceMatrix, const glm::vec3& cameraPosition, const ORIBLData& environmentIBLData, const OEDirectionalLight& directionalLight)
	{
		m_SceneData.ViewProjectionMatrix = viewProjection;
		m_SceneData.CameraPosition = cameraPosition;
		m_SceneData.EnvironmentIBLData = environmentIBLData;
        m_SceneData.LightSpaceMatrix = lightSpaceMatrix;
        m_SceneData.DirectionalLight = directionalLight;
	}

	void ORRenderer::EndScene() noexcept
	{

	}

    void ORRenderer::Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const Ref<AssetManagement::OEMaterial>& material, const glm::mat4& transform)
    {
        OE_CORE_ASSERT(shader, "Submit received a null shader!");
        OE_CORE_ASSERT(mesh, "Submit received a null mesh!");
        OE_CORE_ASSERT(material, "Submit received a null material!");
        OE_CORE_ASSERT(m_ShadowMap, "Shadow Map is null!");

        const AssetManagement::OEMaterialData& materialData = material->GetMaterialData();

        shader->Bind();
        shader->SetMat4("u_ViewProjection", m_SceneData.ViewProjectionMatrix);
        shader->SetMat4("u_Transform", transform);
        shader->SetMat4("u_LightSpaceMatrix", m_SceneData.LightSpaceMatrix);
        shader->SetVec3("u_LightDirection", m_SceneData.DirectionalLight.Direction);
        shader->SetVec3("u_LightColor", m_SceneData.DirectionalLight.Color);
        shader->SetFloat("u_LightIntensity", m_SceneData.DirectionalLight.Intensity);

        shader->SetVec4("u_AlbedoColor", materialData.AlbedoColor);
        shader->SetFloat("u_Metallic", materialData.Metallic);
        shader->SetFloat("u_Roughness", materialData.Roughness);
        shader->SetFloat("u_AmbientOcclusion", materialData.AmbientOcclusion);
        shader->SetVec3("u_CameraPosition", m_SceneData.CameraPosition);

        const ORIBLData& environmentIBLData = m_SceneData.EnvironmentIBLData;

        if (environmentIBLData.IrradianceCubemap)
            environmentIBLData.IrradianceCubemap->Bind(1);

        if (environmentIBLData.PrefilteredCubemap)
            environmentIBLData.PrefilteredCubemap->Bind(2);

        if (environmentIBLData.BRDFLUT)
            environmentIBLData.BRDFLUT->Bind(3);

        m_ShadowMap->BindDepthTexture(4);

        shader->SetInt("u_IrradianceMap", 1);
        shader->SetInt("u_PrefilteredMap", 2);
        shader->SetInt("u_BRDFLUT", 3);
        shader->SetInt("u_ShadowMap", 4);

        if (environmentIBLData.PrefilteredCubemap)
            shader->SetFloat("u_MaxReflectionLOD", static_cast<float>(m_SceneData.EnvironmentIBLData.PrefilteredCubemap->GetMipLevels() - 1));
        else
            shader->SetFloat("u_MaxReflectionLOD", 0.0f);

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

    void ORRenderer::Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const glm::mat4& transform)
    {
        OE_CORE_ASSERT(shader, "Shader is null!");
        OE_CORE_ASSERT(mesh, "Mesh is null!");

        shader->Bind();
        shader->SetMat4("u_Transform", transform);

        mesh->Bind();
        ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
    }

    void ORRenderer::Submit(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh)
    {
        OE_CORE_ASSERT(shader, "Shader is null!");
        OE_CORE_ASSERT(mesh, "Mesh is null!");

        shader->Bind();

        mesh->Bind();
        ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
    }

    void ORRenderer::SubmitSkybox(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const Ref<ORCubemap>& cubemap, const glm::mat4& viewProjection)
    {
        OE_CORE_ASSERT(shader, "Skybox shader is null!");
        OE_CORE_ASSERT(mesh, "Skybox mesh is null!");
        OE_CORE_ASSERT(cubemap, "Skybox cubemap is null!");

        ORRenderCommand::SetDepthFunctionLessOrEqual();

        shader->Bind();
        shader->SetMat4("u_ViewProjection", viewProjection);
        cubemap->Bind(0);

        mesh->Bind();
        ORRenderCommand::DrawIndexed(mesh->GetIndexCount());

        ORRenderCommand::SetDepthFunctionLess();
    }

    glm::mat4 ORRenderer::CalculateLightSpaceMatrix(const OEDirectionalLight& directionalLight) noexcept
    {
        if (directionalLight.Direction == glm::vec3(0.0f))
            return glm::mat4(1.0f);

        const glm::vec3 lightDirection = glm::normalize(directionalLight.Direction);

        constexpr float projectionSize = 20.0f;
        constexpr float nearPlane = 0.1f;
        constexpr float farPlane = 100.0f;

        const glm::vec3 lightTarget = glm::vec3(0.0f);
        const glm::vec3 lightPosition = lightTarget - lightDirection * 50.0f;

        const glm::vec3 up = std::abs(glm::dot(lightDirection, glm::vec3(0.0f, 1.0f, 0.0f))) > 0.99f ? glm::vec3(0.0f, 0.0f, 1.0f) : glm::vec3(0.0f, 1.0f, 0.0f);

        const glm::mat4 lightView = glm::lookAt(lightPosition, lightTarget, up);

        const glm::mat4 lightProjection = glm::ortho(-projectionSize, projectionSize, -projectionSize, projectionSize, nearPlane, farPlane);

        return lightProjection * lightView;
    }

    void ORRenderer::BeginShadowPass()
    {
        OE_CORE_ASSERT(m_ShadowMap, "Shadow Map is null!");

        m_ShadowMap->Bind();

        ORRenderCommand::SetViewport(0, 0, m_ShadowMap->GetResolution(), m_ShadowMap->GetResolution());
        ORRenderCommand::ClearDepth();
    }
    
    void ORRenderer::InitShadowMap(uint32_t resolution)
    {
        OE_CORE_ASSERT(resolution > 0, "Shadow Map resolution must be greater than zero!");

        m_ShadowMap = ORShadowMap::CreateShadowMap();

        OE_CORE_ASSERT(m_ShadowMap, "Failed to create Shadow Map!");

        m_ShadowMap->Init(resolution);
    }

    void ORRenderer::SubmitShadow(const Ref<ORShader>& shader, const Ref<ORMesh>& mesh, const glm::mat4& transform, const glm::mat4& lightSpace)
    {
        OE_CORE_ASSERT(shader, "Shadow Shader is null!");
        OE_CORE_ASSERT(mesh, "Shadow Mesh is null!");

        shader->Bind();
        shader->SetMat4("u_Model", transform);
        shader->SetMat4("u_LightSpaceMatrix", lightSpace);

        mesh->Bind();
        ORRenderCommand::DrawIndexed(mesh->GetIndexCount());
    }

    void ORRenderer::EndShadowPass() noexcept
    {
        OE_CORE_ASSERT(m_ShadowMap, "Shadow Map is null!");

        m_ShadowMap->Unbind();

        ORRenderCommand::SetViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    }
}