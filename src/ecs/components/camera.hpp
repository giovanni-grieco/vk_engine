#pragma once

#include <glm/glm.hpp>

namespace engine
{
    const glm::vec3 worldUp {0.0f, 1.0f, 0.0f};
    struct CameraComponent
    {
        glm::vec3 position{0.0f, 0.0f, 2.0f};
        glm::vec3 forward{0.0f, 0.0f, -1.0f};
        glm::vec3 up {0.0f, 1.0f, 0.0f};

        float fov = 45;
        float nearPlane = 0.1;
        float farPlane = 100;
    };
}