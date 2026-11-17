#pragma once

#include "ORIBLData.h"
#include "ORIBLSettings.h"

#include <OrionEngine/OrionRenderer/ORRenderer.h>

namespace OrionEngine::OrionRenderer
{
	class ORIBLGenerator
	{
	public:

		explicit ORIBLGenerator(ORRenderer& renderer) 
			: m_Renderer(renderer)
		{ }

		ORIBLData Generate(const Ref<ORTexture>& environmentTexture);

	private:

		Ref<ORCubemap> GenerateEnvironmentCubemap(const Ref<ORTexture>& environmentTexture);
		Ref<ORCubemap> GenerateIrradianceCubemap(const Ref<ORCubemap>& environmentCubemap);
		Ref<ORCubemap> GeneratePrefilteredCubemap(const Ref<ORCubemap>& environmentCubemap);
		Ref<ORTexture> GenerateBRDFLUT();

	private:

		ORIBLSettings m_IBLSettings;
		ORRenderer& m_Renderer;
	};
}