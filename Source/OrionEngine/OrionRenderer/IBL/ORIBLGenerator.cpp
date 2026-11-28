#include "ORIBLGenerator.h"
#include "ORIBLSettings.h"

#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/OrionRenderer/ORCubemap.h>
#include <OrionEngine/OrionRenderer/ORCubemapFramebuffer.h>
#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>
#include <OrionEngine/OrionRenderer/ORMesh.h>
#include <OrionEngine/OrionRenderer/ORTexture.h>
#include <OrionEngine/OrionRenderer/OROpenGLShaderAPI.h>
#include <OrionEditor/EditorSpecificRendering/OEGridRenderer.h> // for Utils::ReadShaderFile()
#include <OrionEngine/OrionRenderer/ORTextureFramebuffer.h>
#include <OrionEngine/OrionRenderer/ORIndexBuffer.h>
#include <OrionEngine/OrionRenderer/ORVertexBuffer.h>
#include <OrionEngine/OrionRenderer/ORVertexBufferLayout.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OrionEngine::OrionRenderer
{
    ORIBLData ORIBLGenerator::Generate(const Ref<ORTexture>& environmentTexture)
    {
        OE_CORE_ASSERT(environmentTexture, "Environment texture is null!");

        ORIBLData IBLData;

        IBLData.EnvironmentTexture = environmentTexture;
        IBLData.EnvironmentCubemap = GenerateEnvironmentCubemap(environmentTexture);

        if (!IBLData.EnvironmentCubemap)
            return {};

        IBLData.IrradianceCubemap = GenerateIrradianceCubemap(IBLData.EnvironmentCubemap);
        if (!IBLData.IrradianceCubemap)
            return {};

        IBLData.PrefilteredCubemap = GeneratePrefilteredCubemap(IBLData.EnvironmentCubemap);
        if (!IBLData.PrefilteredCubemap)
            return {};

        IBLData.BRDFLUT = GenerateBRDFLUT();
        if (!IBLData.BRDFLUT)
            return {};

        return IBLData;
    }

    Ref<ORCubemap> ORIBLGenerator::GenerateEnvironmentCubemap(const Ref<ORTexture>& environmentTexture)
    {
        Ref<ORCubemap> environmentCubemap = ORCubemap::CreateCubemap(m_IBLSettings.EnvironmentResolution, {});
        Ref<ORCubemapFramebuffer> environmentCubemapFramebuffer = ORCubemapFramebuffer::CreateCubemapFramebuffer();

        Ref<ORMesh> cubeMesh = ORDefaultRendererResources::GetDefaultMesh();

        OE_CORE_ASSERT(environmentCubemap, "Environment cubemap is null!");
        OE_CORE_ASSERT(environmentCubemapFramebuffer, "Environment cubemap framebuffer is null!");
        OE_CORE_ASSERT(cubeMesh, "Cube mesh is null!");

        if (environmentTexture->GetWidth() == 0 || environmentTexture->GetHeight() == 0)
        {
            ORION_ENGINE_ERROR("Environment texture dimensions are invalid!");
            return nullptr;
        }

        const glm::vec3 captureDirections[] = {
            { 1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f }
        };

        const glm::vec3 captureUpVectors[] = {
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f }
        };

        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        Ref<ORShader> conversionShader = ORShader::CreateShader();

        std::string vertexShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\EquirectangularToCubemapVS.glsl");
        std::string fragmentShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\EquirectangularToCubemapFS.glsl");

        conversionShader->CompileAndLinkThroughShaderFiles(vertexShaderSource, fragmentShaderSource);

        for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
        {
            environmentCubemapFramebuffer->AttachCubemapFace(environmentCubemap, faceIndex);
            environmentCubemapFramebuffer->Bind();

            const glm::mat4 captureView = glm::lookAt(
                glm::vec3(0.0f),
                captureDirections[faceIndex],
                captureUpVectors[faceIndex]
            );

            conversionShader->Bind();
            conversionShader->SetMat4("u_View", captureView);
            conversionShader->SetMat4("u_Projection", captureProjection);

            environmentTexture->Bind(0);
            conversionShader->SetInt("u_EquirectangularMap", 0);

            m_Renderer.Submit(conversionShader, cubeMesh);

            environmentCubemapFramebuffer->Unbind();
        }

        return environmentCubemap;
    }

    Ref<ORCubemap> ORIBLGenerator::GenerateIrradianceCubemap(const Ref<ORCubemap>& environmentCubemap)
    {
        if (!environmentCubemap)
        {
            ORION_ENGINE_ERROR("Environment cubemap is null!");
            return nullptr;
        }

        Ref<ORCubemap> irradianceCubemap = ORCubemap::CreateCubemap(m_IBLSettings.IrradianceResolution, {});
        Ref<ORCubemapFramebuffer> irradianceCubemapFramebuffer = ORCubemapFramebuffer::CreateCubemapFramebuffer();
        Ref<ORMesh> cubeMesh = ORDefaultRendererResources::GetDefaultMesh();

        OE_CORE_ASSERT(irradianceCubemap, "Irradiance cubemap is null!");
        OE_CORE_ASSERT(irradianceCubemapFramebuffer, "Irradiance cubemap framebuffer is null!");
        OE_CORE_ASSERT(cubeMesh, "Cube mesh is null!");

        const glm::vec3 captureDirections[] = {
            { 1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f }
        };

        const glm::vec3 captureUpVectors[] = {
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f }
        };

        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        Ref<ORShader> irradianceShader = ORShader::CreateShader();

        std::string vertexShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\IrradianceConvolutionVS.glsl");

        std::string fragmentShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\IrradianceConvolutionFS.glsl");

        irradianceShader->CompileAndLinkThroughShaderFiles(vertexShaderSource, fragmentShaderSource);

        irradianceShader->Bind();
        irradianceShader->SetInt("u_EnvironmentMap", 0);

        environmentCubemap->Bind();

        for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
        {
            irradianceCubemapFramebuffer->AttachCubemapFace(irradianceCubemap, faceIndex);
            irradianceCubemapFramebuffer->Bind();

            const glm::mat4 captureView = glm::lookAt(
                glm::vec3(0.0f),
                captureDirections[faceIndex],
                captureUpVectors[faceIndex]
            );

            irradianceShader->SetMat4("u_View", captureView);
            irradianceShader->SetMat4("u_Projection", captureProjection);

            // Clear the current face before rendering.
            // Ensure the appropriate OpenGL command is available here.
            m_Renderer.Submit(irradianceShader, cubeMesh);

            irradianceCubemapFramebuffer->Unbind();
        }

        irradianceShader->Unbind();
        environmentCubemap->Unbind();

        return irradianceCubemap;
    }

    Ref<ORCubemap> ORIBLGenerator::GeneratePrefilteredCubemap(const Ref<ORCubemap>& environmentCubemap)
    {
        OE_CORE_ASSERT(environmentCubemap, "Environment cubemap is null!");
        environmentCubemap->GenerateMipmaps();

        Ref<ORCubemap> prefilteredCubemaps = ORCubemap::CreateCubemap(m_IBLSettings.PrefilterResolution, {}, m_IBLSettings.PrefilterMipLevels);
        Ref<ORCubemapFramebuffer> prefilteredCubemapFramebuffers = ORCubemapFramebuffer::CreateCubemapFramebuffer();
        Ref<ORMesh> cubeMesh = ORDefaultRendererResources::GetDefaultMesh();

        OE_CORE_ASSERT(prefilteredCubemaps, "Prefiltered cubemaps is null!");
        OE_CORE_ASSERT(prefilteredCubemapFramebuffers, "Prefiltered cubemap framebuffers is null!");
        OE_CORE_ASSERT(cubeMesh, "Cube mesh is null!");

        std::string vertexShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\PrefilterEnvironmentVS.glsl");
        std::string fragmentShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\PrefilterEnvironmentFS.glsl");

        Ref<ORShader> prefilterShader = ORShader::CreateShader();

        prefilterShader->CompileAndLinkThroughShaderFiles(vertexShaderSource, fragmentShaderSource);
        prefilterShader->Bind();

        const glm::vec3 captureDirections[] = {
            { 1.0f,  0.0f,  0.0f },
            {-1.0f,  0.0f,  0.0f },
            { 0.0f,  1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f }
        };

        const glm::vec3 captureUpVectors[] = {
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f,  0.0f,  1.0f },
            { 0.0f,  0.0f, -1.0f },
            { 0.0f, -1.0f,  0.0f },
            { 0.0f, -1.0f,  0.0f }
        };

        const uint32_t mipLevels = m_IBLSettings.PrefilterMipLevels;
        const glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

        if (mipLevels < 2)
        {
            ORION_ENGINE_ERROR("Prefilter mip level count must be at least 2!");
            return nullptr;
        }

        for (uint32_t mipLevel = 0; mipLevel < mipLevels; mipLevel++)
        {
            const float roughness = static_cast<float>(mipLevel) / static_cast<float>(mipLevels - 1);

            for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
            {
                prefilteredCubemapFramebuffers->AttachCubemapFace(prefilteredCubemaps, faceIndex, mipLevel);

                if (!prefilteredCubemapFramebuffers->IsComplete())
                {
                    ORION_ENGINE_ERROR("Prefiltered cubemap framebuffers is incomplete!");
                    return nullptr;
                }

                prefilteredCubemapFramebuffers->Bind();

                const uint32_t mipResolution = std::max(1u, m_IBLSettings.PrefilterResolution >> mipLevel);

                ORRenderCommand::SetViewport(0, 0, static_cast<int>(mipResolution), static_cast<int>(mipResolution));

                const glm::mat4 captureView = glm::lookAt(glm::vec3(0.0f), captureDirections[faceIndex], captureUpVectors[faceIndex]);

                prefilterShader->SetMat4("u_View", captureView);
                prefilterShader->SetMat4("u_Projection", captureProjection);
                prefilterShader->SetFloat("u_Roughness", roughness);
                prefilterShader->SetFloat("u_EnvironmentResolution", static_cast<float>(environmentCubemap->GetWidth()));
                prefilterShader->SetInt("u_EnvironmentMap", 0);

                environmentCubemap->Bind();
                m_Renderer.Submit(prefilterShader, cubeMesh);

                environmentCubemap->Unbind();
                prefilteredCubemapFramebuffers->Unbind();
            }
        }

        return prefilteredCubemaps;
    }

    Ref<ORTexture> ORIBLGenerator::GenerateBRDFLUT()
    {
        Ref<ORTexture> brdfLUT = ORTexture::CreateTexture(m_IBLSettings.BRDFLUTResolution, m_IBLSettings.BRDFLUTResolution);
        OE_CORE_ASSERT(brdfLUT, "Failed to create BRDF LUT texture!");

        Ref<ORTextureFramebuffer> framebuffer = ORTextureFramebuffer::CreateTextureFramebuffer();
        OE_CORE_ASSERT(framebuffer, "Failed to create BRDF LUT framebuffer!");

        Ref<ORShader> BRDFShader = ORShader::CreateShader();
        OE_CORE_ASSERT(BRDFShader, "Failed to create BRDF LUT shader!");

        framebuffer->AttachTexture(brdfLUT);
        OE_CORE_ASSERT(framebuffer->IsComplete(), "BRDF LUT framebuffer is incomplete!");

        std::string vertexShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\BRDFIntegrationVS.glsl");
        std::string fragmentShaderSource = Utils::ReadShaderFile("D:\\dev\\OrionEngine\\Source\\OrionEngine\\OrionRenderer\\IBL\\Shaders\\BRDFIntegrationFS.glsl");

        BRDFShader->CompileAndLinkThroughShaderFiles(vertexShaderSource, fragmentShaderSource);

        struct FullscreenVertex
        {
            glm::vec2 Position;
        };

        const std::vector<FullscreenVertex> vertices =
        {
            { { -1.0f, -1.0f } },
            { {  3.0f, -1.0f } },
            { { -1.0f,  3.0f } }
        };

        const std::vector<uint32_t> indices = { 0, 1, 2 };

        Ref<ORVertexArray> vertexArray = ORVertexArray::CreateVertexArray();
        Ref<ORVertexBuffer> vertexBuffer = ORVertexBuffer::CreateVertexBuffer(vertices.data(), static_cast<uint32_t>(vertices.size() * sizeof(FullscreenVertex)));
        Ref<ORIndexBuffer> indexBuffer = ORIndexBuffer::CreateIndexBuffer(indices.data(), static_cast<uint32_t>(indices.size()));

        OE_CORE_ASSERT(vertexArray, "Failed to create BRDF LUT vertex array!");
        OE_CORE_ASSERT(vertexBuffer, "Failed to create BRDF LUT vertex buffer!");
        OE_CORE_ASSERT(indexBuffer, "Failed to create BRDF LUT index buffer!");

        ORVertexBufferLayout layout;
        layout.Push<glm::vec2>("a_Position");

        vertexBuffer->SetLayout(layout);
        vertexArray->AddVertexBuffer(vertexBuffer);
        vertexArray->SetIndexBuffer(indexBuffer);

        Ref<ORMesh> fullscreenTriangle = CreateRef<ORMesh>(vertexArray);

        framebuffer->Bind();
        ORRenderCommand::SetViewport(0, 0, m_IBLSettings.BRDFLUTResolution, m_IBLSettings.BRDFLUTResolution);
        ORRenderCommand::Clear();

        m_Renderer.Submit(BRDFShader, fullscreenTriangle);

        framebuffer->Unbind();

        return brdfLUT;
    }
}