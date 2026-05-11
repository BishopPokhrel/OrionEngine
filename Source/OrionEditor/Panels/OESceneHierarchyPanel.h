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
		struct CreationDataNeeded
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

		struct DeletionDataNeeded
		{
			char GameEntityNameBufferToDelete[1024];
			bool OpenGameEntityDeletionDialog = false; 
			bool OpenComponentDeletionDialog = false; 
			char GameEntityNameInWhichComponentWillBeDeleted[1024];
			int ComponentDeletionIndex = 0;
		};
		
		Scope<CreationDataNeeded> CreationDataNeeded;
		Scope<DeletionDataNeeded> DeletionDataNeeded;
	};

	class OESceneHierarchyPanel
	{
	public:

		bool ShowSceneHierachy();
		bool DrawSceneHierarchyPanel();
		bool DrawCommandPalette();

	private:

		ECS::OEECSRegistry* m_Registry = nullptr;
		OEEditorContext* m_EditorContext = nullptr;
		OESHPDataNeeded m_OESHPDataNeeded;
		bool m_OpenCommandPalette;
	};
}