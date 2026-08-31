#include "vk_host_buffer.hpp"
#include <cstring>
#include <stdexcept>

namespace engine
{
    // NOTE: this duplicates the one in vk_buffer.cpp — extract it (see below).
    static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter,
                                   VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
                return i;
        }
        throw std::runtime_error("failed to find suitable memory type!");
    }

    VulkanHostBuffer::VulkanHostBuffer(VulkanDevice &device, VkDeviceSize size, uint32_t framesInFlight,
                                       VkBufferUsageFlags usage)
        : device_(device.device), size_(size), entries_(framesInFlight)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        for (auto &entry : entries_)
        {
            if (vkCreateBuffer(device_, &bufferInfo, nullptr, &entry.buffer) != VK_SUCCESS)
                throw std::runtime_error("failed to create host buffer!");

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(device_, entry.buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo{};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(
                device.physicalDevice,
                memRequirements.memoryTypeBits,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

            if (vkAllocateMemory(device_, &allocInfo, nullptr, &entry.memory) != VK_SUCCESS)
                throw std::runtime_error("failed to allocate host buffer memory!");

            vkBindBufferMemory(device_, entry.buffer, entry.memory, 0);
            vkMapMemory(device_, entry.memory, 0, size, 0, &entry.mapped);
        }
    }

    VulkanHostBuffer::~VulkanHostBuffer()
    {
        for (auto &entry : entries_)
        {
            if (entry.mapped) vkUnmapMemory(device_, entry.memory);
            if (entry.buffer) vkDestroyBuffer(device_, entry.buffer, nullptr);
            if (entry.memory) vkFreeMemory(device_, entry.memory, nullptr);
        }
    }

    void VulkanHostBuffer::update(uint32_t frameIndex, const void *data, VkDeviceSize size)
    {
        memcpy(entries_[frameIndex].mapped, data, size); // host-coherent => visible next submission
    }
}