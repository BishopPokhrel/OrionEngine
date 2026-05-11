#pragma once

#include <OrionEditor/OEEditorContext.h>

// ECS includes
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
{
	struct OEDPDataNeeded
	{
		struct GameEntityDataNeeded
		{
			char GameEntityNameBuffer[1024];
		};

		struct ComponentDataNeeded
		{
			int ComponentTypeCreationIndex = 0;
			int ComponentTypeDeletionIndex = 0;
		};

		Scope<GameEntityDataNeeded> GameEntityDataNeeded;
		Scope<ComponentDataNeeded> ComponentDataNeeded;
	};

	class OEDetailsPanel
	{
	public:

		bool DrawEntityDetails();
		bool DrawDetailsPanel();

	private:

		OEEditorContext* m_EditorContext = nullptr;
		ECS::OEECSRegistry* m_Registry = nullptr;
		OEDPDataNeeded m_OEDPDataNeeded;
	};
}