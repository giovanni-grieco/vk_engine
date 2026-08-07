#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <set>
#include <iostream>

#include "vk_instance.hpp"
#include "vk_surface.hpp"
#include "vk_queue_indices.hpp"


namespace engine{
    class VulkanDevice{
        public:
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkDevice device = VK_NULL_HANDLE;
            VulkanDevice(VulkanInstance& instance, VulkanSurface& surface, bool enableValidationLayers, const std::vector<const char *>& validationLayers);
            ~VulkanDevice();
        private:
            void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
            
    };
}