#pragma once

#include <glm/glm.hpp>

namespace OrionEngine::OrionRenderer
{
	struct SceneData
	{
		glm::mat4 ViewProjectionMatrix;
		glm::vec3 CameraForward;
	};
}