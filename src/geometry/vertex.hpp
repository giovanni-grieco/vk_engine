#pragma once

#include <glm/glm.hpp>

namespace engine{
    struct Vertex{
        glm::vec2 pos;
        glm::vec3 color;
    };

    const std::vector<Vertex> triangle1 = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    const std::vector<Vertex> triangle2 = {
        {{0.0f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}}
    };

    const std::vector<Vertex> triangle3 = {
        {{0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}},
        {{0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}}
    };


}