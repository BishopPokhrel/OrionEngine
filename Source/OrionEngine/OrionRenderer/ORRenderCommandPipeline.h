#pragma once

#include <vector>

#include <OrionEngine/Core/OERef.h>

// Renderer Includes
#include "ORShader.h"
#include "ORVertexArray.h"
#include "ORRenderCmd.h"

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
		void BeginScene(const OEScene& scene) noexcept;
		void BeginFrame() noexcept;
		void SubmitToGPU(const Ref<ORShader>& shader, const Ref<ORVertexArray>& vertexArray, const glm::mat4& transform) noexcept;
		void EndFrame() noexcept;
		void EndScene(const OEScene& scene) noexcept;
		void Execute() noexcept;

	private:

		std::vector<ORRenderCmd> m_CommandQueue;
	};
}