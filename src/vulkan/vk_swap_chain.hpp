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
        std::vector<VkImage> images;
        std::vector<VkImageView> imageViews;
        VkFormat format;
        VkExtent2D extent;
        VulkanSwapChain(VulkanDevice &device, VulkanSurface &surface, Window &window);
        ~VulkanSwapChain();
        void recreate(VulkanDevice &device, VulkanSurface &surface, Window &window);

    private:
        void init(VulkanDevice &device, VulkanSurface &surface, Window &window);
        void destroy();
    };
}