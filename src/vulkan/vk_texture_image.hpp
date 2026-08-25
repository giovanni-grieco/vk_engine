#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstddef>

#include "vk_device.hpp"
#include "vk_command_pool.hpp"
#include "vk_queues.hpp"

namespace engine
{
    class VulkanTextureImage
    {
    public:
        // Non-owned context handles.
        VkDevice device = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkQueue graphicsQueue = VK_NULL_HANDLE;

        // Owned GPU resources.
        VkImage image = VK_NULL_HANDLE;
        VkDeviceMemory imageMemory = VK_NULL_HANDLE;
        VkImageView imageView = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        uint32_t width = 0;
        uint32_t height = 0;

        VulkanTextureImage(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues);
        ~VulkanTextureImage();

        VulkanTextureImage(const VulkanTextureImage &other) = delete;
        VulkanTextureImage(VulkanTextureImage &&other) noexcept;

        VulkanTextureImage &operator=(const VulkanTextureImage &other) = delete;
        VulkanTextureImage &operator=(VulkanTextureImage &&other) noexcept;

        // Allocates the device-local image, plus its image view and sampler.
        void create(uint32_t width, uint32_t height);

        // Stages the given CPU pixel data through a temporary staging buffer,
        // copies it into the image and transitions it to shader-read-only layout.
        void upload(const void *data, size_t size);

        // Frees all GPU resources (called by the destructor too).
        void destroy();

    private:
        void transitionImageLayout(VkCommandBuffer commandBuffer, VkImageLayout oldLayout, VkImageLayout newLayout);
        void copyBufferToImage(VkCommandBuffer commandBuffer, VkBuffer srcBuffer);
    };
}