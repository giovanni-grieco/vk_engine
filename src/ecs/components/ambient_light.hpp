#pragma once

#include <glm/glm.hpp>

namespace engine{
    struct AmbientLightComponent{
        float intensity = 0.1f;
        glm::vec3 color = glm::vec3(1.0f);
    };
}