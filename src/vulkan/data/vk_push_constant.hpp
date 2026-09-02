#pragma once

#include <glm/glm.hpp>

namespace engine
{
    // Per-draw transform, pushed via vkCmdPushConstants.
    struct ModelPushConstant
    {
        glm::mat4 model{};
        glm::mat4 normal{};
        glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f}; // rgb = diffuse color multiplier
        int hasTexture = 1;                       // 1 = sample texSampler, 0 = vertex color only
    };
}