#pragma once

#include <glm/vec3.hpp>

#include <iostream>

namespace engine
{
    struct TransformComponent
    {

        glm::vec3 position{};
        glm::vec3 rotation{};
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        void dump()
        {
            std::cout << "Transform: \n";
            std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
            std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
            std::cout << "Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")\n";
        }
    };
}