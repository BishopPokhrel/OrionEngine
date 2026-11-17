#pragma once

#include <cstdint>

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORIndexBuffer
		{
		public:

			virtual ~ORIndexBuffer() = default;

			virtual void Bind() const noexcept = 0;
			virtual void Unbind() const noexcept = 0;

			virtual uint32_t GetCount() const noexcept = 0;

			static Ref<ORIndexBuffer> CreateIndexBuffer(const uint32_t* indices, uint32_t count);
		};
	}
}