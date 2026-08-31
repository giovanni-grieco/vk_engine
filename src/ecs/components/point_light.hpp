#pragma once

#include <glm/glm.hpp>

namespace engine
{
    struct PointLightComponent
    {
        glm::vec3 color = glm::vec3(1.0f);
        float intensity = 1.0f;
    };
}
