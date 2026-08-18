#pragma once

#include <glm/glm.hpp>

namespace engine
{
    struct CameraComponent
    {
        glm::vec3 position;
        glm::vec3 up;
        glm::vec3 forward;
    };
}