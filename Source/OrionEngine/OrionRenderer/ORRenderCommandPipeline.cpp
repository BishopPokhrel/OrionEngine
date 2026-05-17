#include "ORRenderCommandPipeline.h"

#include <OrionEngine/Core/Scene/OEScene.h>

namespace OrionEngine::OrionRenderer
{
	ORRenderCmdType ORRenderCommandPipeline::RegisterCmdType(ORRenderCmdType type) noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = type;
		return type;
	}

	void ORRenderCommandPipeline::BeginFrame() noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = ORRenderCmdType::BeginFrame;
		m_CommandQueue.push_back(cmd);
	}

	void ORRenderCommandPipeline::BeginScene(const OEScene& scene) noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = ORRenderCmdType::BeginScene;
		m_CommandQueue.push_back(cmd);
	}

	void ORRenderCommandPipeline::SubmitToGPU(const Ref<ORShader>& shader, const Ref<ORVertexArray>& vertexArray, const glm::mat4& transform) noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = ORRenderCmdType::DrawMesh;

		cmd.DrawCommand.Shader = shader;
		cmd.DrawCommand.VertexArray = vertexArray;
		cmd.DrawCommand.Transform = transform;

		m_CommandQueue.push_back(cmd);
	}

	void ORRenderCommandPipeline::EndFrame() noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = ORRenderCmdType::EndFrame;
		m_CommandQueue.push_back(cmd);
	}

	void ORRenderCommandPipeline::EndScene(const OEScene& scene) noexcept
	{
		ORRenderCmd cmd{};
		cmd.Type = ORRenderCmdType::EndScene;
		m_CommandQueue.push_back(cmd);
	}

	void ORRenderCommandPipeline::ExecuteClearCmd()
	{
		ORRenderCommand::Clear();
	}

	void ORRenderCommandPipeline::ExecuteClearColorCmd(float r, float g, float b, float a)
	{
		ORRenderCommand::SetClearColor(r, g, b, a);
	}
}
