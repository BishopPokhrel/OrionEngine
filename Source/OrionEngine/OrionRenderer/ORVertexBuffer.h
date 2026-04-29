#pragma once

#include "ORGraphicsAPI.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"
#include "ORRenderer.h"

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

			static Ref<ORVertexBuffer> Create(float* vertices, uint32_t size) noexcept
			{
				OE_CORE_ASSERT(vertices, "Vertex data is null!");
				OE_CORE_ASSERT(size > 0, "VertexBuffer size must be > 0");

				switch (ORRenderer::GetGraphicsAPI())
				{
					case ORGraphicsAPI::None:
						OE_CORE_ASSERT(false, "Graphics API is not set!");
						return nullptr;

					case ORGraphicsAPI::OpenGL:
						return CreateRef<OpenGLVertexBuffer>(vertices, size);
				}

				OE_CORE_ASSERT(false, "Unknown graphics API!");
				return nullptr;
			}
		};
	}
} 