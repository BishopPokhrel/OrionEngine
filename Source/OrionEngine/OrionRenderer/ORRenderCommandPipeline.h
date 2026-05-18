#pragma once

#include <vector>

#include <OrionEngine/Core/OERef.h>

// Renderer Includes
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORRenderCmd.h"
#include "ORRenderer.h"

#include <glm/glm.hpp>

namespace OrionEngine
{
	class OEScene;
}

namespace OrionEngine::OrionRenderer
{
	class ORRenderCommandPipeline
	{
	public:

		ORRenderCmdType RegisterCmdType(ORRenderCmdType type) noexcept;
		void BeginScene(const glm::mat4& viewProjection) noexcept;
		void BeginFrame() noexcept;
		void SubmitToGPU(const Ref<ORShader>& shader, const Ref<ORVertexArray>& vertexArray, const glm::mat4& transform) noexcept;
		void EndFrame() noexcept;
		void EndScene() noexcept;
		
		void ExecuteClearCmd();
		void ExecuteClearColorCmd(float r, float g, float b, float a);

	private:

		std::vector<ORRenderCmd> m_CommandQueue;
		ORRenderer* m_Renderer = nullptr;
	};
}