#pragma once

#include <glm/glm.hpp>

namespace engine
{

    // Per-draw transform, pushed via vkCmdPushConstants.
    struct ModelPushConstant
    {
        glm::mat4 model;
    };
}