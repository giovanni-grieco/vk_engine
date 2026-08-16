#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include "vk_device.hpp"

namespace engine{

    enum class BufferType{
        VERTEX,
        STAGING,
        INDEX
    };


    class VulkanBuffer{
        public:
            VkDevice device = VK_NULL_HANDLE;
            VkBuffer buffer = VK_NULL_HANDLE;
            VkDeviceMemory bufferMemory = VK_NULL_HANDLE;
            size_t size;

            VulkanBuffer(VulkanDevice& device, BufferType type, size_t size, void* data);
            ~VulkanBuffer();

            VulkanBuffer(const VulkanBuffer& other) = delete;
            VulkanBuffer(VulkanBuffer&& other) noexcept;

            VulkanBuffer& operator=(const VulkanBuffer& other) = delete;
            VulkanBuffer& operator=(VulkanBuffer&& other) noexcept;

            //void map(void* data, size_t size);

        private:
            void destroy();

    };
}