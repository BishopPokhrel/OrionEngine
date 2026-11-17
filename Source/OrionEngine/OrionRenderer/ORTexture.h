#pragma once

#include <cstdint>
#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/AssetManagement/DefaultEngineAssetTypes/OETexture.h>

namespace OrionEngine::OrionRenderer
{
	class ORTexture
	{
	public:

		virtual ~ORTexture() = default;

		virtual void Bind(uint32_t textureSlot = 0) const noexcept = 0;
		virtual void Unbind() const noexcept = 0;

		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;

		static Ref<ORTexture> CreateTexture(const AssetManagement::OETextureData& textureData); 
		static Ref<ORTexture> CreateTexture(uint32_t width, uint32_t height);
	};
}