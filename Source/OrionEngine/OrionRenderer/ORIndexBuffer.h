#pragma once

#include <cstdint>

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
		};
	}
}