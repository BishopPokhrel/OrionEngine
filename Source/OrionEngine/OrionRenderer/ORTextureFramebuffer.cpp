#include "ORTextureFramebuffer.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLTextureFramebuffer.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORTextureFramebuffer> ORTextureFramebuffer::CreateTextureFramebuffer() noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLTextureFramebuffer>();
		}

		return nullptr;
	}
}