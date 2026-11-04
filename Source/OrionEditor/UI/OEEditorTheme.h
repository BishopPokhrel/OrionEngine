#pragma once

#include <imgui.h>

#include <OrionEditor/EditorSpecificRendering/OpenGLUsage/OEEditorIconRenderer.h>
#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionEditor
{
	class OEEditorTheme
	{
	public:

		static void ApplyTheme();
		static void LoadFonts();

		static void LoadEditorIcon(const std::string& iconName);
		static unsigned int GetEditorIcon(const std::string& iconName);

		static ImFont* RegularFont; 
		static ImFont* MediumFont;
		static ImFont* SemiBoldFont;
		static ImFont* ItalicFont;

	private:

		static Scope<EditorSpecificRendering::OEEditorIconRenderer> m_EditorIconRenderer;
	};
}