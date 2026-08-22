#pragma once

#include <stdint.h>
#include <vector>

#include <glm/glm.hpp>

#include "vertex.hpp"

namespace engine{

    typedef int32_t MeshID;

    struct Mesh{
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
    };

    // Command flowing from game logic down to the Vulkan renderer.
    struct DrawPacket{
        MeshID meshId = -1;
        glm::mat4 model = glm::mat4(1.0f);
    };
}