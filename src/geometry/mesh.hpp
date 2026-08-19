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

    // GPU-side draw data for a mesh stored in the unified vertex/index buffers.
    struct MeshDrawInfo{
        uint32_t indexCount = 0;   // number of indices to draw
        uint32_t firstIndex = 0;   // offset into the unified index buffer (in indices)
        int32_t  vertexOffset = 0; // added to every index value (vkCmdDrawIndexed vertexOffset)
    };

    // Command flowing from game logic down to the Vulkan renderer.
    struct DrawPacket{
        MeshID meshId = -1;
        glm::mat4 model = glm::mat4(1.0f);
    };
}