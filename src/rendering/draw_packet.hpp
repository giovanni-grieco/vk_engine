#pragma once

#include "geometry/mesh.hpp"

namespace engine{

    // Command flowing from game logic down to the Vulkan renderer.
    struct DrawPacket{
        MeshID meshId = -1;
        glm::mat4 model = glm::mat4(1.0f);
    };
}