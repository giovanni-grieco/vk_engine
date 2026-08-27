#pragma once

#include <vulkan/vulkan.h>

#include "vk_device.hpp"

namespace engine
{
    // Owns the depth image used as the depth/stencil attachment of the render pass.
    class VulkanDepthBuffer
    {
    public:
        VkDevice device = VK_NULL_HANDLE;
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory memory = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkFormat format = VK_FORMAT_UNDEFINED;

        VulkanDepthBuffer(VulkanDevice &device, uint32_t width, uint32_t height);
        ~VulkanDepthBuffer();

        VulkanDepthBuffer(const VulkanDepthBuffer &) = delete;
        VulkanDepthBuffer &operator=(const VulkanDepthBuffer &) = delete;

        // Recreates the depth image for a new extent (e.g. after a swapchain resize).
        void recreate(VulkanDevice &device, uint32_t width, uint32_t height);

        VkImageView view() const { return imageView; }
        VkFormat depthFormat() const { return format; }

    private:
        void init(VulkanDevice &device, uint32_t width, uint32_t height);
        void destroy();
    };
}
