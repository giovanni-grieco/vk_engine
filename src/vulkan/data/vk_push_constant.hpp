#pragma once

#include <glm/glm.hpp>

namespace engine
{
    // Per-draw transform, pushed via vkCmdPushConstants.
    struct ModelPushConstant
    {
        glm::mat4 model{};
        glm::vec4 normal[3]{}; // 3x3 normal matrix, one column per vec4 (w unused)
        glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f}; // rgb = diffuse color multiplier
    };
}