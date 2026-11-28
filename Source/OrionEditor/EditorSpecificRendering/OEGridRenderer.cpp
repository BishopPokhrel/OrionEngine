#include "OEGridRenderer.h"

#include <OrionEngine/Core/OEAssert.h>
#include <OrionEngine/Core/OELogging.h>
#include <OrionEngine/OrionRenderer/ORRenderCommand.h>
#include <OrionEngine/OrionRenderer/OROpenGLShaderAPI.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>

#include <fstream>
#include <sstream>
#include <string>

#include <glm/gtc/matrix_inverse.hpp>

namespace OrionEngine::OrionEditor::EditorSpecificRendering
{
    void OEGridRenderer::Init()
    {
        m_FullscreenVAO = OrionRenderer::ORVertexArray::CreateVertexArray();
        m_GridShader = OrionRenderer::ORShader::CreateShader();

        std::string vertexShaderSource = Utils::ReadShaderFile(
            "D:/dev/OrionEngine/Source/OrionEditor/EditorSpecificRendering/Shaders/GridVertexShader.glsl"
        );

        std::string fragmentShaderSource = Utils::ReadShaderFile(
            "D:/dev/OrionEngine/Source/OrionEditor/EditorSpecificRendering/Shaders/GridFragmentShader.glsl"
        );

        OE_CORE_ASSERT(!vertexShaderSource.empty(), "Grid vertex shader is empty!");
        OE_CORE_ASSERT(!fragmentShaderSource.empty(), "Grid fragment shader is empty!");

        m_GridShader->CompileAndLinkThroughShaderFiles(vertexShaderSource, fragmentShaderSource);
    }

    void OEGridRenderer::RenderGrid(const OrionRenderer::ORCamera& camera)
    {
        OE_CORE_ASSERT(m_GridShader, "Grid shader is null!");
        OE_CORE_ASSERT(m_FullscreenVAO, "Fullscreen vertex array is null!");

        m_GridShader->Bind();

        m_GridShader->SetMat4("u_InverseViewProjection", glm::inverse(camera.GetViewProjection()));
        m_GridShader->SetMat4("u_ViewProjection", camera.GetViewProjection());
        m_GridShader->SetVec3("u_CameraPosition", camera.GetPosition());
        m_GridShader->SetFloat("u_GridSpacing", 1.0f);
        m_GridShader->SetFloat("u_FadeStart", 150.0f);
        m_GridShader->SetFloat("u_FadeEnd", 400.0f);

        m_FullscreenVAO->Bind();

        OrionRenderer::ORRenderCommand::DrawArrays(3);

        m_FullscreenVAO->Unbind();
        m_GridShader->Unbind();
    }
}