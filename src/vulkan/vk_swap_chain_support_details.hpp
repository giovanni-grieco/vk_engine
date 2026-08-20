#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "platform/window.hpp"

namespace engine
{
    struct VulkanSwapChainSupportDetails
    {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        VkSurfaceFormatKHR chooseSwapSurfaceFormat();
        VkPresentModeKHR choosePresentMode();
        VkExtent2D chooseSwapExtent(Window &window);
    };

    VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
}