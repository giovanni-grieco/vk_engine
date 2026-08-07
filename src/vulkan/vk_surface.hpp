#pragma once

#include <vulkan/vulkan.h>

#include "platform/window.hpp"
#include "vulkan/vk_instance.hpp"

namespace engine{
    class VulkanSurface{
        public:
            VkInstance instance;
            VkSurfaceKHR surface = VK_NULL_HANDLE;
            VulkanSurface(VulkanInstance& instance, Window& window);
            ~VulkanSurface();
    };
}