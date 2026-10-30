#pragma once

#include <imgui.h>

namespace OrionEngine::OrionEditor
{
	class OEEditorTheme
	{
	public:

		static void ApplyTheme();
		static void LoadFonts();

		static ImFont* RegularFont; 
		static ImFont* MediumFont;
		static ImFont* SemiBoldFont;
		static ImFont* ItalicFont;
	};
}