#pragma once

#include <glm/vec3.hpp>

#include <iostream>

namespace engine{
    struct Transform{

        glm::vec3 position;
        glm::vec3 rotation;

        void dump(){
            std::cout << "Transform: \n";
            std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
            std::cout << "Rotation: (" << rotation.x << ", " << rotation.y << ", " << rotation.z << ")\n";
        }
    };
}