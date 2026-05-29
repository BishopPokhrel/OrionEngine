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

		using CreationData = CreationDataNeeded;
		using DeletionData = DeletionDataNeeded;
		using EditableData = EditableDataNeeded;

		Scope<CreationData> CreationDataNeeded = CreateScope<CreationData>();
		Scope<DeletionData> DeletionDataNeeded = CreateScope<DeletionData>();
		Scope<EditableData> EditableDataNeeded = CreateScope<EditableData>();
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

		OEScene m_CurrentScene;
		std::string m_SceneName;
		OESMPDataNeeded m_OESMPDataNeeded;
	};
}