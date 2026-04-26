#pragma once

#include "ORGraphicsAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include <OrionEngine/Core/OERef.h>

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

			static Ref<ORVertexBuffer> Create(float* vertices, uint32_t size) noexcept;

		private:

			ORGraphicsAPI m_GraphicsAPI;

		};
	}
} 