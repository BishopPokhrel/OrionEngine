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
		constexpr OEGameEntityID INVALID_GAME_ENTITY_ID = 0;
	}
}