#pragma once

#include "geometry/mesh.hpp"
#include "texture/texture.hpp"

namespace engine{

    // Command flowing from game logic down to the Vulkan renderer.
    struct DrawPacket{
        MeshID meshId = -1;
        uint32_t subMeshIndex = 0; // which material group of the mesh to draw
        TextureID textureId = -1;
        glm::vec4 color = glm::vec4(1.0f); // per-material diffuse color multiplier
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 normal = glm::mat4(1.0f);
    };
}