#include "ORTexture.h"

#include "ORRenderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORTexture> ORTexture::CreateTexture(const AssetManagement::OETextureData& textureData)
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API is not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLTexture>(textureData);
		}

		return nullptr;
	}
}