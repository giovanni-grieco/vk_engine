#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>

namespace engine{
    struct LocalTransformComponent{
        glm::vec3 position{};
        glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale{1.0f, 1.0f, 1.0f};

        void dump()
        {
            glm::vec3 euler = glm::degrees(glm::eulerAngles(rotation));
            std::cout << "Transform: \n";
            std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
            std::cout << "Rotation: (" << euler.x << ", " << euler.y << ", " << euler.z << ")\n";
            std::cout << "Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")\n";
        }
    };
}