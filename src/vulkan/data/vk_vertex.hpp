#pragma once

#include "geometry/vertex.hpp"
#include <vulkan/vulkan.h>
#include <array>

namespace engine
{
    VkVertexInputBindingDescription getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 4> getAttributeDescription();
}