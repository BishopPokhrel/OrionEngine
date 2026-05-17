#include "ORVertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexBuffer.h"

#include "ORRenderer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORVertexBuffer> ORVertexBuffer::Create(const void* vertices, uint32_t size) 
	{
		switch (m_Renderer->GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				OE_CORE_ASSERT(false, "Unknown graphics API!");

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}
	}
}