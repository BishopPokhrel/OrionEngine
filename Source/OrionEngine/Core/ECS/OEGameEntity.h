#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace OrionEngine
{
	namespace ECS
	{
		using OEGameEntityID = uint64_t;
		constexpr OEGameEntityID MAX_GAME_ENTITIES = 50000;
	}
}