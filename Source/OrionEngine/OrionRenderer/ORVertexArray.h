#pragma once

#include "ORVertexBuffer.h"

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		class ORVertexArray
		{
		public:

			virtual ~ORVertexArray() noexcept = default;
			virtual void Bind() const noexcept = 0;
			virtual void Unbind() const noexcept = 0;
			virtual void AddVertexBuffer(const Ref<ORVertexBuffer>& VBO) noexcept = 0;
		};
	}
}