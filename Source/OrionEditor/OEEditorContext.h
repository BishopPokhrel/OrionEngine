#pragma once

#include <OrionEngine/Core/ECS/OEGameEntity.h>

namespace OrionEngine::OrionEditor
{
	struct OEEditorContext
	{
		ECS::OEGameEntityID SelectedEntity = ECS::INVALID_GAME_ENTITY_ID;
	};
}