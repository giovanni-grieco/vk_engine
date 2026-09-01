#pragma once

#include <glm/glm.hpp>

namespace engine{
    struct DirectionalLightComponent{
        float intensity = 0.2f;
        glm::vec3 direction {};
        glm::vec3 color = glm::vec3(1.0f);
    };
}