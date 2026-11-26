#include "ORShadowMap.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLShadowMap.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORShadowMap> ORShadowMap::CreateShadowMap() noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLShadowMap>();
		}
		return nullptr;
	}
}