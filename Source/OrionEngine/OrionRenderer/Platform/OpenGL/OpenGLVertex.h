#pragma once

#include <glm/glm.hpp>

namespace OrionEngine
{
	namespace OrionRenderer
	{
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec2 uv;
		};
	}
}