#include "ORCubemapFramebuffer.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLCubemapFramebuffer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORCubemapFramebuffer> ORCubemapFramebuffer::CreateCubemapFramebuffer() noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API is not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLCubemapFramebuffer>();
		}

		return nullptr;
	}
}