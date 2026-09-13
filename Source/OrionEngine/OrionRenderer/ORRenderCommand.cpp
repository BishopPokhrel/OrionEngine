#include "ORRenderCommand.h"

namespace OrionEngine::OrionRenderer
{
	Scope<OpenGLRendererAPI> ORRenderCommand::s_RendererAPI = CreateScope<OpenGLRendererAPI>();
}