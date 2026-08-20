#pragma once

#include <glm/glm.hpp>

namespace engine{
    struct FrameInfo{
        glm::mat4 view;
        glm::mat4 projection;
    };
}