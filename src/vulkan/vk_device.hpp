#pragma once

#include "vk_instance.hpp"
#include "queue_family.hpp"

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <set>
#include <iostream>

namespace engine{
    class VulkanDevice{
        public:
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkDevice device = VK_NULL_HANDLE;
            VulkanDevice(VkInstance instance, VkSurfaceKHR surface, bool enableValidationLayers, const std::vector<const char *>& validationLayers);
            ~VulkanDevice();
        private:
            void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface);
            
    };
}