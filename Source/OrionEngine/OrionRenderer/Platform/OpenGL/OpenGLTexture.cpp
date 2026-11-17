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
		const size_t expectedElementCount = pixelCount * textureData.Channels;

		if (textureData.IsImageHDR)
		{
			if (textureData.HDRPixelData.size() < expectedElementCount)
			{
				ORION_ENGINE_ERROR("HDR texture pixel data is incomplete!");
				return;
			}
		}
		else
		{
			if (textureData.PixelData.size() < expectedElementCount)
			{
				ORION_ENGINE_ERROR("Texture pixel data is incomplete!");
				return;
			}
		}

		GLenum sourceFormat = GL_NONE;
		GLenum internalFormat = GL_NONE;

		switch (textureData.Channels)
		{
			case 1:
				sourceFormat = GL_RED;
				internalFormat = !textureData.IsImageHDR ? GL_R8 : GL_R16F;
				break;

			case 2:
				sourceFormat = GL_RG;
				internalFormat = !textureData.IsImageHDR ? GL_RG8 : GL_RG16F;
				break;

			case 3:
				sourceFormat = GL_RGB;
				internalFormat = !textureData.IsImageHDR ? GL_RGB8 : GL_RGB16F;
				break;

			case 4:
				sourceFormat = GL_RGBA;
				internalFormat = !textureData.IsImageHDR ? GL_RGBA8 : GL_RGBA16F;
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, textureData.IsImageHDR ? 4 : 1);

		GLenum dataType = textureData.IsImageHDR ? GL_FLOAT : GL_UNSIGNED_BYTE;
		const void* pixelData = textureData.IsImageHDR ? static_cast<const void*>(textureData.HDRPixelData.data()) : static_cast<const void*>(textureData.PixelData.data());

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(textureData.Width), static_cast<GLsizei>(textureData.Height), 0, sourceFormat, dataType, pixelData);

		glPixelStorei(GL_UNPACK_ALIGNMENT, previousUnpackAlignment);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);
		m_Width = textureData.Width;
		m_Height = textureData.Height;
	}

	OpenGLTexture::OpenGLTexture(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0)
		{
			ORION_ENGINE_ERROR("Texture dimensions are invalid: Overload function (width, height)"); // Just to make it more clear
			return;
		}

		GLint maxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		if (maxTextureSize <= 0)
		{
			ORION_ENGINE_ERROR("Failed to retrieve maximum texture size!");
			return;
		}

		if (width > static_cast<uint32_t>(maxTextureSize) || height > static_cast<uint32_t>(maxTextureSize))
		{
			ORION_ENGINE_ERROR("Texture dimensions exceed GPU limits!");
			return;
		}

		glGenTextures(1, &m_RendererID);

		if (m_RendererID == 0)
		{
			ORION_ENGINE_ERROR("Failed to generate OpenGL texture!");
			return;
		}

		glBindTexture(GL_TEXTURE_2D, m_RendererID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, static_cast<GLsizei>(width), static_cast<GLsizei>(height), 0, GL_RG, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);

		m_Width = width;
		m_Height = height;
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
