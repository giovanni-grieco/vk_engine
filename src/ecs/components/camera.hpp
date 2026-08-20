#pragma once

#include <glm/glm.hpp>

namespace engine
{
    struct CameraComponent
    {
        glm::vec3 position{};
        glm::vec3 forward{};
        glm::vec3 up{};

        float fov = 45;
        float nearPlane = 0.1;
        float farPlane = 100;
    };
}