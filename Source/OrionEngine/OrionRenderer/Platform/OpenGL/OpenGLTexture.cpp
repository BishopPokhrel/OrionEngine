#include "OpenGLTexture.h"

#include <OrionEngine/Core/OELogging.h>

#include <limits>

namespace OrionEngine::OrionRenderer
{
	OpenGLTexture::OpenGLTexture(const AssetManagement::OETextureData& textureData)
	{
		if (textureData.Width == 0 || textureData.Height == 0)
		{
			ORION_ENGINE_ERROR("Texture dimensions are invalid!");
			return;
		}

		if (textureData.Channels < 1 || textureData.Channels > 4)
		{
			ORION_ENGINE_ERROR("Channels are not between 1-4!");
			return;
		}

		if (textureData.IsImageHDR)
		{
			ORION_ENGINE_WARNING("HDR Textures are not supported yet!");
			return;
		}

		if (textureData.Width > static_cast<uint32_t>(std::numeric_limits<GLsizei>::max()) || textureData.Height > static_cast<uint32_t>(std::numeric_limits<GLsizei>::max()))
		{
			ORION_ENGINE_ERROR("Texture dimensions exceed OpenGL limits!");
			return;
		}

		const size_t maxSize = std::numeric_limits<size_t>::max();

		if (static_cast<size_t>(textureData.Width) > maxSize / textureData.Height)
		{
			ORION_ENGINE_ERROR("Texture dimensions overflow!");
			return;
		}

		const size_t pixelCount = static_cast<size_t>(textureData.Width) * textureData.Height;

		if (textureData.Channels > maxSize / pixelCount)
		{
			ORION_ENGINE_ERROR("Texture pixel count overflow!");
			return;
		}

		const size_t expectedByteCount = pixelCount * textureData.Channels;

		if (textureData.PixelData.size() < expectedByteCount)
		{
			ORION_ENGINE_ERROR("Texture pixel data is incomplete!");
			return;
		}

		GLenum sourceFormat = GL_NONE;
		GLenum internalFormat = GL_NONE;

		switch (textureData.Channels)
		{
		case 1:
			sourceFormat = GL_RED;
			internalFormat = GL_R8;
			break;

		case 2:
			sourceFormat = GL_RG;
			internalFormat = GL_RG8;
			break;

		case 3:
			sourceFormat = GL_RGB;
			internalFormat = GL_RGB8;
			break;

		case 4:
			sourceFormat = GL_RGBA;
			internalFormat = GL_RGBA8;
			break;
		}

		glGenTextures(1, &m_RendererID);

		if (m_RendererID == 0)
		{
			ORION_ENGINE_ERROR("Failed to generate OpenGL texture!");
			return;
		}

		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		GLint previousUnpackAlignment = 4;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousUnpackAlignment);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(textureData.Width), static_cast<GLsizei>(textureData.Height), 0, sourceFormat, GL_UNSIGNED_BYTE, textureData.PixelData.data());

		glPixelStorei(GL_UNPACK_ALIGNMENT, previousUnpackAlignment);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Width = textureData.Width;
		m_Height = textureData.Height;
	}

	OpenGLTexture::~OpenGLTexture()
	{
		if (m_RendererID != 0)
		{
			glDeleteTextures(1, &m_RendererID);
			m_RendererID = 0;
		}
	}

	void OpenGLTexture::Bind(uint32_t textureSlot) const noexcept
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	}

	void OpenGLTexture::Unbind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
