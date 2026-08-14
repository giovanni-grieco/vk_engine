#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vk_device.hpp"

namespace engine{
    class VulkanSyncObjects{
        public:
            VkDevice device;
            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;

            VulkanSyncObjects(size_t framesInFlight, size_t swapchainImageCount, VulkanDevice& device);
            ~VulkanSyncObjects();
    };
}