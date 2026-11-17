#pragma once

#include <OrionEngine/Core/OERef.h>

#include <cstdint>
#include <vector>

namespace OrionEngine::OrionRenderer
{
	class ORCubemap
	{
	public:

		virtual ~ORCubemap() = default;

		virtual void Bind(uint32_t textureSlot = 0) const noexcept = 0;
		virtual void Unbind() const noexcept = 0;
		virtual void GenerateMipmaps() const noexcept = 0;
		
		virtual uint32_t GetMipLevels() const noexcept = 0;
		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;

		static Ref<ORCubemap> CreateCubemap(uint32_t resolution, const std::vector<float>& cubemapData, uint32_t mipLevels = 1) noexcept;
	};
}