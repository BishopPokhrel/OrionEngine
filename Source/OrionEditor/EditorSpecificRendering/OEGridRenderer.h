#pragma once

#include <glm/glm.hpp>

#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/OrionRenderer/ORShader.h>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OELogging.h>

#include <string>
#include <fstream>
#include <sstream>

namespace Utils
{
    inline std::string ReadShaderFile(const std::string& filepath)
    {
        std::ifstream file(filepath);

        if (!file.is_open())
        {
            ORION_ENGINE_ERROR("Failed to open shader file: ", filepath);
            return {};
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        return buffer.str();
    }
}

namespace OrionEngine::OrionEditor::EditorSpecificRendering
{
	class OEGridRenderer
	{
	public:

		void Init();
		void RenderGrid(const OrionRenderer::ORCamera& camera);

	private:

		Ref<OrionRenderer::ORShader> m_GridShader;
		Ref<OrionRenderer::ORVertexArray> m_FullscreenVAO;
	};
}