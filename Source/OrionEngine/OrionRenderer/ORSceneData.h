#pragma once

#include <glm/glm.hpp>

namespace OrionEngine::OrionRenderer
{
	struct OESceneData
	{
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 CameraPosition;
	};
}