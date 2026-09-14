#pragma once

#include <cstdint>

#include <OrionEngine/Core/OERef.h>

#include <OrionEngine/OrionRenderer/ORGraphicsAPI.h>

namespace OrionEngine::OrionRenderer
{
	class ORFrameBuffer
	{
	public:

		virtual ~ORFrameBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		
		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;

		virtual uint32_t GetColorAttachementRendererID() const noexcept = 0;
		virtual uint32_t GetDepthAttachementRendererID() const noexcept = 0;

		static Ref<ORFrameBuffer> CreateFrameBuffer(uint32_t width, uint32_t height, ORGraphicsAPI api);
	};
}