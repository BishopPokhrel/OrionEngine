#pragma once

#include <cstdint>

namespace OrionEngine::OrionRenderer
{
    struct ORIBLSettings
    {
        uint32_t EnvironmentResolution = 512;
        uint32_t IrradianceResolution = 32;
        uint32_t PrefilterResolution = 128;
        uint32_t BRDFLUTResolution = 512;

        uint32_t PrefilterMipLevels = 5;
    };
}