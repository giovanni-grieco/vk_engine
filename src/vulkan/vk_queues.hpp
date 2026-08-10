#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

#include "vk_device.hpp"
#include "vk_surface.hpp"

namespace engine
{

    class VulkanQueues
    {
    public:
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        VkQueue presentQueue = VK_NULL_HANDLE;
        VulkanQueues(VulkanDevice &device, VulkanSurface &surface);
    };
}