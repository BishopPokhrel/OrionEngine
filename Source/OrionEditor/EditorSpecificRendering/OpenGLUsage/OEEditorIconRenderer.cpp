#include "OEEditorIconRenderer.h"

#include <OrionEngine/OS/Windows/WindowsOSAPI.h>

namespace OrionEngine::OrionEditor::EditorSpecificRendering
{
    OEEditorIconRenderer::~OEEditorIconRenderer()
    {
        Shutdown();
    }

    void OEEditorIconRenderer::LoadEditorIcon(const std::string& iconName)
    {
        if (m_IconTextures.contains(iconName))
            return;

        unsigned int width = 0;
        unsigned int height = 0;

        const std::filesystem::path iconPath = std::filesystem::path("Assets/Icons") / (iconName + ".png");

        std::vector<unsigned char> pixels = OS::LoadWindowsImage(iconPath, width, height);

        if (pixels.empty())
            return;

        GLuint textureID = 0;

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

        glBindTexture(GL_TEXTURE_2D, 0);

        m_IconTextures.emplace(iconName, textureID);
    }

    GLuint OEEditorIconRenderer::GetIcon(const std::string& iconName) const
    {
        const auto iterator = m_IconTextures.find(iconName);

        if (iterator == m_IconTextures.end())
            return 0;

        return iterator->second;
    }

    void OEEditorIconRenderer::Shutdown()
    {
        for (const auto& [iconName, textureID] : m_IconTextures)
            glDeleteTextures(1, &textureID);

        m_IconTextures.clear();
    }
}