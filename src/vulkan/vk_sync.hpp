#pragma once

#include <vulkan/vulkan.h>
#include "vk_device.hpp"

namespace engine{
    class VulkanSyncObjects{
        public:
            VkDevice device;
            VkSemaphore imageAvailable;
            VkSemaphore renderFinished;
            VkFence inFlight;

            VulkanSyncObjects(VulkanDevice& device);
            ~VulkanSyncObjects();
    };
}