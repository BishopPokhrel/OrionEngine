#pragma once

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine::OrionEditor
{
	class OEConsolePanel
	{
	public:

		bool DrawConsolePanel();

	private:

		bool ShowOutputLog();
		bool DrawSearchBar();

		bool DrawSelectedLogView();
		
		struct OECPDN // OECPDN = Orion Editor Console Panel Data Needed 
		{
			bool AutoScroll = true;
			char SearchBuffer[1024] = {};
			OELogging::OELogEntry CurrentlySelectedLog;
		};

	private:

		OECPDN m_OECPDN;
	};
}