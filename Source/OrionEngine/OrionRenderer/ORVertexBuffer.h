#pragma once

#include "ORGraphicsAPI.h"
#include "ORRenderer.h"
#include "ORVertexBufferLayout.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORVertexBuffer
		{
		public:

			virtual ~ORVertexBuffer() noexcept = default;
			virtual void Bind() const noexcept = 0;
			virtual void Unbind() const noexcept = 0;

			virtual void SetLayout(const ORVertexBufferLayout& layout) noexcept = 0;
			virtual const ORVertexBufferLayout& GetLayout() const noexcept = 0;

			static Ref<ORVertexBuffer> Create(const void* vertices, uint32_t size);
		};
	}
} 