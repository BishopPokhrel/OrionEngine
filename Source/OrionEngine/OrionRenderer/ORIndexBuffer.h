#pragma once

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
		};
	}
}