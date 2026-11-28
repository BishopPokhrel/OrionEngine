#include "ORShader.h"

#include "ORRenderer.h"
#include "OROpenGLShaderAPI.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORShader> ORShader::CreateShader() noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OROpenGLShaderAPI>();
		}

		return nullptr;
	}
}