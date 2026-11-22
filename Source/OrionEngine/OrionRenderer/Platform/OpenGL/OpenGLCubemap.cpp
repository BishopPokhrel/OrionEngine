#include "OpenGLCubemap.h"

#include <OrionEngine/Core/OELogging.h>

#include <limits>
#include <algorithm>

namespace OrionEngine::OrionRenderer
{
    OpenGLCubemap::OpenGLCubemap(uint32_t resolution, const std::vector<float>& cubemapData, uint32_t mipLevels)
    {
        if (resolution == 0)
        {
            ORION_ENGINE_ERROR("Resolution is 0!");
            return;
        }

        if (resolution > static_cast<uint32_t>(std::numeric_limits<GLsizei>::max()))
        {
            ORION_ENGINE_ERROR("Resolution boundaries exceeded!");
            return;
        }

        GLint maxCubemapSize = 0;
        glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &maxCubemapSize);

        if (maxCubemapSize <= 0)
        {
            ORION_ENGINE_ERROR("Failed to retrieve maximum cubemap size!");
            return;
        }

        if (resolution > static_cast<uint32_t>(maxCubemapSize))
        {
            ORION_ENGINE_ERROR("Cubemap resolution exceeds GPU limits!");
            return;
        }

        const GLenum sourceFormat = GL_RGB;
        const GLenum internalFormat = GL_RGB16F;

        const size_t maxSize = std::numeric_limits<size_t>::max();

        if (static_cast<size_t>(resolution) > maxSize / resolution)
        {
            ORION_ENGINE_ERROR("Cubemap pixel count overflow!");
            return;
        }

        const size_t facePixelCount = static_cast<size_t>(resolution) * resolution;

        if (facePixelCount > maxSize / 3)
        {
            ORION_ENGINE_ERROR("Cubemap face element count overflow!");
            return;
        }

        const size_t faceElementCount = facePixelCount * 3;

        if (faceElementCount > maxSize / 6)
        {
            ORION_ENGINE_ERROR("Cubemap total element count overflow!");
            return;
        }

        const size_t expectedElementCount = faceElementCount * 6;

        const bool hasValidData = cubemapData.size() == expectedElementCount;

        if (!cubemapData.empty() && !hasValidData)
        {
            ORION_ENGINE_ERROR("Cubemap data size is invalid!");
            return;
        }

        uint32_t maxMipLevels = 1;

        for (uint32_t mipResolution = resolution; mipResolution > 1; mipResolution /= 2)
            maxMipLevels++;

        if (mipLevels == 0 || mipLevels > maxMipLevels)
        {
            ORION_ENGINE_ERROR("Cubemap mip level count is invalid!");
            return;
        }

        glGenTextures(1, &m_RendererID);

        if (m_RendererID == 0)
        {
            ORION_ENGINE_ERROR("Failed to generate OpenGL cubemap!");
            return;
        }

        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

        for (uint32_t faceIndex = 0; faceIndex < 6; faceIndex++)
        {
            uint32_t mipResolution = resolution;

            for (uint32_t mipLevel = 0; mipLevel < mipLevels; mipLevel++)
            {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, static_cast<GLint>(mipLevel), internalFormat, static_cast<GLsizei>(mipResolution), static_cast<GLsizei>(mipResolution), 0, sourceFormat, GL_FLOAT, mipLevel == 0 && hasValidData ? cubemapData.data() + (static_cast<size_t>(faceIndex) * faceElementCount) : nullptr);
                mipResolution = std::max(1u, mipResolution / 2);
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, mipLevels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(mipLevels - 1));

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        m_Width = resolution;
        m_Height = resolution;
        m_MipLevels = mipLevels;
    }

    OpenGLCubemap::~OpenGLCubemap()
    {
        if (m_RendererID != 0)
        {
            glDeleteTextures(1, &m_RendererID);
            m_RendererID = 0;
        }
    }

    void OpenGLCubemap::Bind(uint32_t textureSlot) const noexcept
    {
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
    }

    void OpenGLCubemap::Unbind() const noexcept
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }

    void OpenGLCubemap::GenerateMipmaps() const noexcept
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}