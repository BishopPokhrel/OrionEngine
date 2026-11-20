#pragma once

#include <cstdint>
#include <limits>

namespace OrionEngine::OrionRenderer
{
    struct ORIBLSettings
    {
        uint32_t EnvironmentResolution = 512;
        uint32_t IrradianceResolution = 32;
        uint32_t PrefilterResolution = 128;
        uint32_t BRDFLUTResolution = 512;

        uint32_t PrefilterMipLevels = 5;

        void SetEnvironmentResolution(uint32_t environmentResolution) noexcept
        {
            if (environmentResolution > std::numeric_limits<uint32_t>::max() || environmentResolution == 0)
                return;
            EnvironmentResolution = environmentResolution;
        }

        void SetIrradianceResolution(uint32_t irradianceResolution) noexcept
        {
            if (irradianceResolution > std::numeric_limits<uint32_t>::max() || irradianceResolution == 0)
                return;
            IrradianceResolution = irradianceResolution;
        }

        void SetPrefilterResolution(uint32_t prefilterResolution) noexcept
        {
            if (prefilterResolution > std::numeric_limits<uint32_t>::max() || prefilterResolution == 0)
                return;
            PrefilterResolution = prefilterResolution;
        }

        void SetPrefilterMipLevels(uint32_t prefilterMipLvls) noexcept
        {
            if (prefilterMipLvls > std::numeric_limits<uint32_t>::max() || prefilterMipLvls == 0)
                return;
            PrefilterMipLevels = prefilterMipLvls;
        }
    };
}