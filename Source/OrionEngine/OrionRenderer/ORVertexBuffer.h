#pragma once

#include "ORGraphicsAPI.h"
#include "ORVertexBufferLayout.h"

#include <OrionEngine/Core/OERef.h>
#include <OrionEngine/Core/OEAssert.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORRenderer; // forward declare

		class ORVertexBuffer
		{
		public:

			virtual ~ORVertexBuffer() noexcept = default;
			virtual void Bind() const noexcept = 0;
			virtual void Unbind() const noexcept = 0;

			virtual void SetLayout(const ORVertexBufferLayout& layout) noexcept = 0;
			virtual const ORVertexBufferLayout& GetLayout() const noexcept = 0;

			Ref<ORVertexBuffer> Create(const void* vertices, uint32_t size);

		private:

			ORRenderer* m_Renderer = nullptr;
		};
	}
} 