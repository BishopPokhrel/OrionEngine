#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>

#include <unordered_map>
#include <string>

namespace OrionEngine::OrionEditor::EditorSpecificRendering
{
	class OEEditorIconRenderer
	{
	public:

		OEEditorIconRenderer() = default;
		~OEEditorIconRenderer();

		void LoadEditorIcon(const std::string& iconName);
		
		GLuint GetIcon(const std::string& iconName) const; 
		
		void Shutdown();

	private:

		std::unordered_map<std::string, GLuint> m_IconTextures;
	
	};
}