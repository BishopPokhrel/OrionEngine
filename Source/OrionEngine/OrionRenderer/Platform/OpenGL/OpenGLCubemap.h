#pragma once

#define GLFW_INCLUDE_NONE

#include <OrionEngine/OrionRenderer/ORCubemap.h>

#include <glad/glad.h>

namespace OrionEngine::OrionRenderer
{
	class OpenGLCubemap : public ORCubemap
	{
	public:

		OpenGLCubemap(uint32_t resolution, const std::vector<float>& cubemapData, uint32_t mipLevels = 1);
		~OpenGLCubemap();

		void Bind(uint32_t textureSlot = 0) const noexcept override;
		void Unbind() const noexcept override;
		void GenerateMipmaps() const noexcept override;

		uint32_t GetWidth() const noexcept override { return m_Width; }
		uint32_t GetHeight() const noexcept override { return m_Height; }
		uint32_t GetMipLevels() const noexcept override { return m_MipLevels; }

	private:

		GLuint m_RendererID = 0;

		uint32_t m_Width = 0;
		uint32_t m_Height = 0;
		uint32_t m_MipLevels = 1;

		friend class OpenGLCubemapFramebuffer;
	};
}