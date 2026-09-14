#include "ORFramebuffer.h"

#include "ORGraphicsAPI.h"
#include "ORRenderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORFrameBuffer> ORFrameBuffer::CreateFrameBuffer(uint32_t width, uint32_t height, ORGraphicsAPI api)
	{
		switch (api)
		{
			case ORGraphicsAPI::None:
			{
				OE_CORE_ASSERT(false, "Attempted to create framebuffer without a valid Graphics API");
				return nullptr;
			}
			case ORGraphicsAPI::OpenGL:
			{
				return CreateRef<OpenGLFrameBuffer>(width, height);
			}
		}
	}
}