#pragma once

#include <OrionEditor/OEEditorContext.h>
#include <OrionEditor/OEEditorRenderCommands.h>

// ECS includes
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEngine/Core/Scene/OEScene.h>

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
{
	struct OEDPDataNeeded
	{
		struct OGameEntityDataNeeded
		{
			char GameEntityNameBuffer[1024];
		};

		struct OComponentDataNeeded
		{
			int ComponentTypeCreationIndex = 0;
			int ComponentTypeDeletionIndex = 0;
		};

		struct OMaterialDataNeeded
		{
			int MaterialTypeCreationIndex = 0;
		};

		Scope<OGameEntityDataNeeded> GameEntityDataNeeded = CreateScope<OGameEntityDataNeeded>();
		Scope<OComponentDataNeeded> ComponentDataNeeded = CreateScope<OComponentDataNeeded>();
		Scope<OMaterialDataNeeded> MaterialDataNeeded = CreateScope<OMaterialDataNeeded>();
	};

	class OEDetailsPanel
	{
	public:

		explicit OEDetailsPanel(OEEditorContext* context)
			: m_EditorContext(context)
		{ }

		void SetRegistry(ECS::OEECSRegistry& registry) { m_Registry = &registry; }
		void SetScene(OEScene* scene) { m_Scene = scene; }

		bool DrawEntityDetails();
		bool DrawDetailsPanel();

	private:

		OEEditorContext* m_EditorContext = nullptr;
		ECS::OEECSRegistry* m_Registry = nullptr;
		OEScene* m_Scene = nullptr;
		OEDPDataNeeded m_OEDPDataNeeded;
	};
}