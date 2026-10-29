#pragma once

#include <glm/glm.hpp>

namespace OrionEngine::OrionRenderer
{
	struct OEDirectionalLight
	{
		glm::vec3 Direction = { -0.5f, -1.0f, -0.5f };
		glm::vec3 Color = { 0.0f, 0.0f, 0.0f };
		float Intensity = 0.0f;
	};
}