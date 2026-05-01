#include "ORVertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORVertexBuffer> ORVertexBuffer::Create(const void* vertices, uint32_t size) 
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				OE_CORE_ASSERT(false, "Unknown graphics API!");

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
	}
}