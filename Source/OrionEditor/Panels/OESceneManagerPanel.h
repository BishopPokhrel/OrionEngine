#pragma once

#include <OrionEngine/Core/Scene/OEScene.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
{
	struct OESMPDataNeeded
	{
		struct CreationDataNeeded
		{
			char NewSceneNameBuffer[1024];
			bool NewSceneDialog = false; 
		};

		struct DeletionDataNeeded
		{
			bool AreYouSureDialog = false; 
		};

		struct EditableDataNeeded
		{
			char EditableSceneNameBuffer[1024];
			
		};

		Scope<CreationDataNeeded> CreationDataNeeded;
		Scope<DeletionDataNeeded> DeletionDataNeeded;
		Scope<EditableDataNeeded> EditableDataNeeded;
	};

	class OESceneManagerPanel
	{
	public:

		bool DrawSceneCreation();
		bool DrawSceneDeletion();
		bool DrawSceneMetadata();
		bool DrawSceneEdit();
		bool DrawSceneManagerPanel();

	private:

		OEScene* m_CurrentScene = nullptr;
		std::string m_SceneName;
		OESMPDataNeeded m_OESMPDataNeeded;
	};
}