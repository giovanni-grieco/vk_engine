#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>
#include <vector>

#include "vk_device.hpp"

namespace engine
{
    // One persistently-mapped, host-visible buffer per frame in flight.
    class VulkanUniformBuffer
    {
    public:
        VulkanUniformBuffer(VulkanDevice &device, VkDeviceSize size, uint32_t framesInFlight);
        ~VulkanUniformBuffer();

        VulkanUniformBuffer(const VulkanUniformBuffer &) = delete;
        VulkanUniformBuffer &operator=(const VulkanUniformBuffer &) = delete;
        VulkanUniformBuffer(VulkanUniformBuffer &&) = delete;
        VulkanUniformBuffer &operator=(VulkanUniformBuffer &&) = delete;

        // Copies CPU data into the buffer for the given frame index.
        void update(uint32_t frameIndex, const void *data, VkDeviceSize size);

        VkBuffer buffer(uint32_t frameIndex) const { return entries_[frameIndex].buffer; }
        VkDeviceSize size() const { return size_; }

    private:
        struct Entry
        {
            VkBuffer buffer = VK_NULL_HANDLE;
            VkDeviceMemory memory = VK_NULL_HANDLE;
            void *mapped = nullptr;
        };

        VkDevice device_ = VK_NULL_HANDLE;
        VkDeviceSize size_ = 0;
        std::vector<Entry> entries_;
    };
}