#pragma once

#include <glm/glm.hpp>

#include "IBL/ORIBLData.h"
#include "OEDirectionalLight.h"

namespace OrionEngine::OrionRenderer
{
    struct OESceneData
    {
        glm::mat4 ViewProjectionMatrix;
        glm::mat4 LightSpaceMatrix = glm::mat4(1.0f);
        glm::vec3 CameraPosition;

        ORIBLData EnvironmentIBLData;
        OEDirectionalLight DirectionalLight;
    };
}