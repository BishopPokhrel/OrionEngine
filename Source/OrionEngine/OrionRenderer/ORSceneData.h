#pragma once

#include <glm/glm.hpp>

#include <OrionEngine/OrionRenderer/IBL/ORIBLData.h>

namespace OrionEngine::OrionRenderer
{
    struct OESceneData
    {
        glm::mat4 ViewProjectionMatrix;
        glm::vec3 CameraPosition;

        ORIBLData EnvironmentIBLData;
    };
}