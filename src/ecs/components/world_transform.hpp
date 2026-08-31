#pragma once

#include <glm/matrix.hpp>

#include <iostream>

namespace engine
{
    struct WorldTransformComponent
    {
        glm::mat4 matrix = glm::mat4(1.0f);
    };
}