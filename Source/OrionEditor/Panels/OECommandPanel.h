#pragma once

#include <OrionEngine/Core/ECS/OEECSRegistry.h>
#include <OrionEditor/OEEditorContext.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
{
	class OECommandPanel
	{
	public:

		explicit OECommandPanel(OEEditorContext* context) noexcept
			: m_EditorContext(context)
		{ }

		void SetRegistry(ECS::OEECSRegistry& registry) noexcept { m_Registry = &registry; }

		bool ShowEntityCommands();
		bool ShowComponentCommands();
		bool DrawOECommandPanel();

	private:

		struct OECPDataNeeded
		{
			struct OGameEntityActionDataNeeded 
			{
				char GameEntityNameBuffer[1024] = {};
				char GameEntityNameBufferForRenaming[1024] = {};
			};

			struct OComponentActionDataNeeded
			{
				int ComponentTypesIndex = 0;
			};

			OGameEntityActionDataNeeded GameEntityActionDataNeeded;
			OComponentActionDataNeeded ComponentActionDataNeeded;
		};

		ECS::OEECSRegistry* m_Registry = nullptr;
		OEEditorContext* m_EditorContext = nullptr;

		OECPDataNeeded m_OECPDataNeeded;
	};
}