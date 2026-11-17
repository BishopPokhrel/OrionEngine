#include "ORIndexBuffer.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORIndexBuffer> ORIndexBuffer::CreateIndexBuffer(const uint32_t* data, uint32_t count)
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLIndexBuffer>(data, count);
		}

		return nullptr;
	}
}