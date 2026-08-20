#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <cstddef>

#include "vk_device.hpp"
#include "vk_command_pool.hpp"
#include "vk_queues.hpp"

namespace engine
{

    enum class BufferType
    {
        VERTEX,
        INDEX
    };

    class VulkanBuffer
    {
    public:
        // Non-owned context handles.
        VkDevice device = VK_NULL_HANDLE;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkCommandPool commandPool = VK_NULL_HANDLE;
        VkQueue transferQueue = VK_NULL_HANDLE;

        // Owned GPU resources.
        VkBuffer buffer = VK_NULL_HANDLE;
        VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
        size_t size = 0;

        VulkanBuffer(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues);
        ~VulkanBuffer();

        VulkanBuffer(const VulkanBuffer &other) = delete;
        VulkanBuffer(VulkanBuffer &&other) noexcept;

        VulkanBuffer &operator=(const VulkanBuffer &other) = delete;
        VulkanBuffer &operator=(VulkanBuffer &&other) noexcept;

        // Allocates the device-local GPU buffer (no data uploaded yet).
        void create(BufferType type, size_t size);

        // Stages the given CPU data into the buffer via a temporary staging buffer.
        // dstOffset is the byte offset inside the destination buffer.
        void upload(const void *data, size_t size, VkDeviceSize dstOffset = 0);

        // Frees the GPU resources (called by the destructor too).
        void destroy();

    private:
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &outBuffer, VkDeviceMemory &outMemory);
        void copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size, VkDeviceSize dstOffset = 0);
    };
}