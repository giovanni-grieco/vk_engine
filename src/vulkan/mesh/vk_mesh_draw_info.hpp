#pragma once

#include <cstdint>

namespace engine{
    // GPU-side draw data for a mesh stored in the unified vertex/index buffers.
    struct MeshDrawInfo{
        uint32_t indexCount = 0;   // number of indices to draw
        uint32_t firstIndex = 0;   // offset into the unified index buffer (in indices)
        int32_t  vertexOffset = 0; // added to every index value (vkCmdDrawIndexed vertexOffset)
    };
}