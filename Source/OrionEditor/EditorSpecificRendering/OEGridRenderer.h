#pragma once

#include <glm/glm.hpp>

#include <OrionEngine/OrionRenderer/SceneRenderer/ORCamera.h>
#include <OrionEngine/OrionRenderer/ORVertexArray.h>
#include <OrionEngine/OrionRenderer/ORShader.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
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