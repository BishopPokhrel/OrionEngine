#pragma once

#include <OrionEngine/OrionRenderer/ORDefaultRendererResources.h>
#include <OrionEngine/Core/ECS/OEComponent.h>

namespace OrionEngine::OrionEditor
{
	class OEEditorRenderCommands
	{
	public:

		static void InitializeRenderableRemaining(ECS::OERenderableComponent& render) noexcept;
	};
}