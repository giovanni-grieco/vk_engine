#pragma once

#include <vulkan/vulkan.h>

namespace engine
{
    class VulkanTextureImage
    {
    public:
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;

        VulkanTextureImage();
        
        VulkanTextureImage(const VulkanTextureImage& other) = delete;
        VulkanTextureImage(VulkanTextureImage&& other) noexcept;

        VulkanTextureImage& operator=(const VulkanTextureImage& other) = delete;
        VulkanTextureImage& operator=(VulkanTextureImage&& other) noexcept;

        ~VulkanTextureImage();

        // Allocates the device-local GPU buffer (no data uploaded yet).
        void create(size_t size);

        // Stages the given CPU data into the buffer via a temporary staging buffer.
        // dstOffset is the byte offset inside the destination buffer.
        void upload(const void *data, size_t size, VkDeviceSize dstOffset = 0);

        // Frees the GPU resources (called by the destructor too).
        void destroy();
        
    };
}