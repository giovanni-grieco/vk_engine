#pragma once

#include "platform/window.hpp"

#include <vulkan/vulkan.h>

namespace engine{
    class VulkanSurface{
        public:
            VkInstance instance;
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            VulkanSurface(VkInstance instance, Window& window);
            ~VulkanSurface();
    };
}