#pragma once

#include "ORCubemap.h"

#include <OrionEngine/Core/OERef.h>

#include <cstdint>

namespace OrionEngine::OrionRenderer
{
	class ORCubemapFramebuffer
	{
	public:

		virtual ~ORCubemapFramebuffer() = default;

		virtual void Bind() const noexcept = 0;
		virtual void Unbind() const noexcept = 0;
		virtual bool IsComplete() const noexcept = 0;

		virtual uint32_t GetWidth() const noexcept = 0;
		virtual uint32_t GetHeight() const noexcept = 0;

		virtual void AttachCubemapFace(const Ref<ORCubemap>& cubemap, uint32_t faceIndex, uint32_t mipLevel = 0) = 0;
		
		static Ref<ORCubemapFramebuffer> CreateCubemapFramebuffer() noexcept;
	};
}