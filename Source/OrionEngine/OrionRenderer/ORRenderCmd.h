// NOTE: THIS FILE IS DIFFERENT FROM ORRenderCommand.h
/*
	This file only contains data of what meshes or entities are to drawn
	It specifices HOW things are to be drawn, what itenaries are needed, etc.
	However, ORRenderCommand.h (NOT THIS FILE, WHICH IS ORRenderCmd.h) contains functions like ClearColor, SetViewport, etc. 
	
	Those are the 'rendering commands'. It helps with the rendering directly

	This file, however does not. As mentioned above, it only specifices the data needed for rendering.
*/

#pragma once

namespace OrionEngine::OrionRenderer
{
	enum class ORRenderCmdType : uint8_t
	{
		None = 0,
		DrawMesh,
		DrawIndexed,

		Clear,
		ClearColor,

		BeginScene,
		BeginFrame,
		EndScene,
		EndFrame
	};

	struct ORDrawCmd
	{
		Ref<ORShader> Shader;
		Ref<ORVertexArray> VertexArray;
		glm::mat4 Transform{ 0.0f };
	};

	struct ORClearColorCmd 
	{
		glm::vec4 Color = { 0.0f, 0.0f, 0.0f, 0.0f };
	};


	struct ORRenderCmd
	{
		ORRenderCmdType Type = ORRenderCmdType::None;

		// Payloads
		ORDrawCmd DrawCommand;
		ORClearColorCmd ClearColorCommand;
	};
}