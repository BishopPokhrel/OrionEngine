#pragma once

#include <OrionEngine/Core/Scene/OEScene.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine::OrionEditor
{
	class OEEnvironmentDetailsPanel
	{
	public:

		explicit OEEnvironmentDetailsPanel(OEScene* scene)
			: m_Scene(scene)
		{
			OE_CORE_ASSERT("Scene is null!");
		}

		bool DrawEnvironmentDetailsPanel();
		
	private:

		struct OEEDPDN // OE Environment Details Panel Data Needed
		{
			char EnvironmentHDRTexturePathBuffer[2048] = {};
			int EnvironmentResolutionIndex = 0;
			int IrradianceResolutionIndex = 0;
			int PrefilterResolutionIndex = 0;
		};

		bool DrawEnvironmentLighting();
		bool DrawIBLData();

		OEEDPDN m_EDPDataNeeded;

	private:

		OEScene* m_Scene = nullptr;
	};
}