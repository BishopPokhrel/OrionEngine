#include "OEEditorRenderCommands.h"

namespace OrionEngine::OrionEditor
{
	void OEEditorRenderCommands::InitializeRenderableRemaining(ECS::OERenderableComponent& render) noexcept
	{
		render.Shader = OrionRenderer::ORDefaultRendererResources::GetDefaultShader();
		render.VertexArray = OrionRenderer::ORDefaultRendererResources::GetDefaultVertexArray();
	}
}