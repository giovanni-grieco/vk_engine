#pragma once

#include <vulkan/vulkan.h>
#include <cstddef>
#include <vector>

#include "vk_device.hpp"

namespace engine
{
    // One persistently-mapped, host-visible buffer per frame in flight.
    // Backs both uniform buffers (UBOs) and storage buffers (SSBOs), depending
    // on the VkBufferUsageFlags passed to the constructor.
    class VulkanHostBuffer
    {
    public:
        VulkanHostBuffer(VulkanDevice &device, VkDeviceSize size, uint32_t framesInFlight,
                         VkBufferUsageFlags usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        ~VulkanHostBuffer();

        VulkanHostBuffer(const VulkanHostBuffer &) = delete;
        VulkanHostBuffer &operator=(const VulkanHostBuffer &) = delete;
        VulkanHostBuffer(VulkanHostBuffer &&) = delete;
        VulkanHostBuffer &operator=(VulkanHostBuffer &&) = delete;

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