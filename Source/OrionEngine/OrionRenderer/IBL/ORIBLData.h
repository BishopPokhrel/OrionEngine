#pragma once

#include <OrionEngine/Core/OERef.h>

namespace OrionEngine::OrionRenderer
{
	// Forward declare classes
	class ORTexture;
	class ORCubemap;

	struct ORIBLData
	{
		Ref<ORCubemap> EnvironmentCubemap;
		Ref<ORCubemap> IrradianceCubemap;
		Ref<ORCubemap> PrefilteredCubemap;

		Ref<ORTexture> BRDFLUT;
		Ref<ORTexture> EnvironmentTexture;
	};
}