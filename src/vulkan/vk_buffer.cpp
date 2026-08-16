#include "vk_buffer.hpp"

namespace engine
{
    uint32_t findMemoryType(VkPhysicalDevice phyDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(phyDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }

    VulkanBuffer::VulkanBuffer(VulkanDevice &device, BufferType type, size_t size, void* data)
    {
        this->device = device.device;
        this->size = size;
        VkBufferCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        createInfo.size = size;
        if (type == BufferType::VERTEX)
        {
            createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
        createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (vkCreateBuffer(device.device, &createInfo, nullptr, &buffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device.device, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(device.physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        if (vkAllocateMemory(device.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate vertex buffer memory!");
        }

        if (vkBindBufferMemory(device.device, buffer, bufferMemory, 0) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to bind buffer memory!");
        }

        void *hostData;
        vkMapMemory(device.device, bufferMemory, 0, size, 0, &hostData);
        memcpy(hostData, data, size);
        vkUnmapMemory(device.device, bufferMemory);
    }

    VulkanBuffer::~VulkanBuffer()
    {
        destroy();
    }

    VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
    {
        this->device = other.device;
        this->buffer = other.buffer;
        other.device = VK_NULL_HANDLE;
        other.buffer = VK_NULL_HANDLE;
    }

    VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept
    {
        if (this != &other)
        {
            this->destroy();
            this->device = other.device;
            this->buffer = other.buffer;
            this->bufferMemory = other.bufferMemory;
            other.device = VK_NULL_HANDLE;
            other.buffer = VK_NULL_HANDLE;
            other.bufferMemory = VK_NULL_HANDLE;
        }
        return *this;
    }

    void VulkanBuffer::destroy()
    {
        if (device != VK_NULL_HANDLE && buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(device, buffer, nullptr);
            vkFreeMemory(device, bufferMemory, nullptr);
        }
        device = VK_NULL_HANDLE;
        buffer = VK_NULL_HANDLE;
        bufferMemory = VK_NULL_HANDLE;
    }

    /*void VulkanBuffer::map(void *data, size_t size)
    {
        void *hostData;
        vkMapMemory(device, bufferMemory, 0, this->size, 0, &hostData);
        memcpy(hostData, data, (size_t) this->size);
        vkUnmapMemory(device, bufferMemory);
    }*/

}