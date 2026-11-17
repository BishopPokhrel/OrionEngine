#include "ORVertexArray.h"

#include "ORRenderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace OrionEngine::OrionRenderer
{
	Ref<ORVertexArray> ORVertexArray::CreateVertexArray() noexcept
	{
		switch (ORRenderer::GetGraphicsAPI())
		{
			case ORGraphicsAPI::None:
				ORION_ENGINE_ERROR("Graphics API not supported!");
				return nullptr;

			case ORGraphicsAPI::OpenGL:
				return CreateRef<OpenGLVertexArray>();
		}
	}
}