#pragma once

// ECS INCLUDES
#include <OrionEngine/Core/ECS/OEGameEntity.h>
#include <OrionEngine/Core/ECS/OEComponent.h>
#include <OrionEngine/Core/ECS/OEECSRegistry.h>

#include <OrionEngine/Core/OERef.h>

#include <OrionEditor/OEEditorContext.h>

namespace OrionEngine::OrionEditor
{
	struct OESHPDataNeeded
	{
		struct OCreationDataNeeded
		{
			bool OpenMaxGameEntitiesReached = false; 
			char GameEntityNameBuffer[1024];
			bool OpenGameEntityNameDialog = false; 
			bool GameEntityCreationSuccess = false; 

			// Components
			bool OpenNewTransformComponentDialog = false; 
			bool OpenNewPhysicsComponentDialog = false; 
			bool OpenNewRenderableComponentDialog = false; 

			char GameEntityNameBufferForTCCreation[1024];
			char GameEntityNameBufferForPCCreation[1024];
			char GameEntityNameBufferForRCCreation[1024];
		};

		struct ODeletionDataNeeded
		{
			char GameEntityNameBufferToDelete[1024];
			bool OpenGameEntityDeletionDialog = false; 
			bool OpenComponentDeletionDialog = false; 
			char GameEntityNameInWhichComponentWillBeDeleted[1024];
			int ComponentDeletionIndex = 0;
		};
		
		Scope<OCreationDataNeeded> CreationDataNeeded = CreateScope<OCreationDataNeeded>();
		Scope<ODeletionDataNeeded> DeletionDataNeeded = CreateScope<ODeletionDataNeeded>();
	};

	class OESceneHierarchyPanel
	{
	public:

		explicit OESceneHierarchyPanel(OEEditorContext* context)
			: m_EditorContext(context)
		{}

		void SetRegistry(ECS::OEECSRegistry& registry) { m_Registry = &registry; }

		bool ShowSceneHierachy();
		bool DrawSceneHierarchyPanel();
		bool DrawCommandPalette();

	private:

		ECS::OEECSRegistry* m_Registry = nullptr;
		OEEditorContext* m_EditorContext = nullptr;
		OESHPDataNeeded m_OESHPDataNeeded;
		bool m_OpenCommandPalette = false;
	};
}