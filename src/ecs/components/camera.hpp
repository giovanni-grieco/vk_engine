#pragma once

#include <glm/glm.hpp>

namespace engine
{
    constexpr glm::vec3 worldUp {0.0f, 1.0f, 0.0f};
    struct CameraComponent
    {
        float fov = 45;
        float nearPlane = 0.1;
        float farPlane = 1000;
    };
}