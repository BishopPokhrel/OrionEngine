#include "ORCubemap.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLCubemap.h"

#include <OrionEngine/Core/OELogging.h>

namespace OrionEngine::OrionRenderer
{
	Ref<ORCubemap> ORCubemap::CreateCubemap(uint32_t resolution, const std::vector<float>& cubemapData, uint32_t mipLevels) noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API is not supported!");
				return nullptr;
		
			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLCubemap>(resolution, cubemapData, mipLevels);
				break;
		}

		return nullptr;
	}
}