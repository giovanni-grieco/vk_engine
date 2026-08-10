#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "platform/window.hpp"
#include "vulkan/vk_device.hpp"

namespace engine
{
    class VulkanSwapChain
    {
    public:
        VkDevice device;
        VkSwapchainKHR swapChain;
        VulkanSwapChain(VulkanDevice &device, VulkanSurface &surface, Window &window);
        ~VulkanSwapChain();
    };
}