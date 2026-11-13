#pragma once

#define GLFW_INCLUDE_NONE

#include <glad/glad.h>

#include <OrionEngine/OrionRenderer/ORTexture.h>

namespace OrionEngine::OrionRenderer
{
	class OpenGLTexture : public ORTexture
	{
	public:

		OpenGLTexture(const AssetManagement::OETextureData& textureData);
		~OpenGLTexture();

		void Bind(uint32_t textureSlot = 0) const noexcept override;
		void Unbind() const noexcept override;

		uint32_t GetWidth() const noexcept override { return m_Width; }
		uint32_t GetHeight() const noexcept override { return m_Height; }

	private:

		GLuint m_RendererID = 0;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
	};
}