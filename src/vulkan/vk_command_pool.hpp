#pragma once

#include <vulkan/vulkan.h>
#include "vk_queue_indices.hpp"
#include "vk_device.hpp"
#include "vk_surface.hpp"

namespace engine{
    class VulkanCommandPool{
        public:
            VkDevice device;
            VkCommandPool commandPool;

            VulkanCommandPool(VulkanDevice& device, VulkanSurface& surface);
            ~VulkanCommandPool();
    };
}