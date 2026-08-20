#pragma once

#include <vulkan/vulkan.h>
#include "vk_queue_indices.hpp"
#include "vk_device.hpp"
#include "vk_surface.hpp"

namespace engine
{
    class VulkanCommandPool
    {
    public:
        VkDevice device;
        VkCommandPool commandPool;

        VulkanCommandPool(VulkanDevice &device, VulkanSurface &surface);
        ~VulkanCommandPool();

        VulkanCommandPool(const VulkanCommandPool &other) = delete;
        VulkanCommandPool &operator=(const VulkanCommandPool &other) = delete;

        VulkanCommandPool(VulkanCommandPool &&other) noexcept;
        VulkanCommandPool &operator=(VulkanCommandPool &&other) noexcept;

    private:
        void destroy();
    };
}