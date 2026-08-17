#include "vk_buffer.hpp"

#include <cstring>

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

    VulkanBuffer::VulkanBuffer(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues)
    {
        this->device = device.device;
        this->physicalDevice = device.physicalDevice;
        this->commandPool = commandPool.commandPool;
        this->transferQueue = queues.graphicsQueue;
    }

    VulkanBuffer::~VulkanBuffer()
    {
        destroy();
    }

    VulkanBuffer::VulkanBuffer(VulkanBuffer &&other) noexcept
        : device(other.device),
          physicalDevice(other.physicalDevice),
          commandPool(other.commandPool),
          transferQueue(other.transferQueue),
          buffer(other.buffer),
          bufferMemory(other.bufferMemory),
          size(other.size)
    {
        other.buffer = VK_NULL_HANDLE;
        other.bufferMemory = VK_NULL_HANDLE;
        other.size = 0;
    }

    VulkanBuffer &VulkanBuffer::operator=(VulkanBuffer &&other) noexcept
    {
        if (this != &other)
        {
            destroy();
            device = other.device;
            physicalDevice = other.physicalDevice;
            commandPool = other.commandPool;
            transferQueue = other.transferQueue;
            buffer = other.buffer;
            bufferMemory = other.bufferMemory;
            size = other.size;
            other.buffer = VK_NULL_HANDLE;
            other.bufferMemory = VK_NULL_HANDLE;
            other.size = 0;
        }
        return *this;
    }

    void VulkanBuffer::create(BufferType type, size_t size)
    {
        this->size = size;

        VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        if (type == BufferType::VERTEX)
        {
            usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
        }
        else if (type == BufferType::INDEX)
        {
            usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
        }

        createBuffer(size, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, buffer, bufferMemory);
    }

    void VulkanBuffer::upload(const void *data, size_t size)
    {
        // Stage the data in a temporary host-visible buffer, then copy it to the device-local buffer.
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingMemory;
        createBuffer(size,
                     VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer,
                     stagingMemory);

        void *mapped;
        vkMapMemory(device, stagingMemory, 0, size, 0, &mapped);
        memcpy(mapped, data, size);
        vkUnmapMemory(device, stagingMemory);

        copyBuffer(stagingBuffer, buffer, size);

        vkDestroyBuffer(device, stagingBuffer, nullptr);
        vkFreeMemory(device, stagingMemory, nullptr);
    }

    void VulkanBuffer::destroy()
    {
        if (buffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(device, buffer, nullptr);
        }
        if (bufferMemory != VK_NULL_HANDLE)
        {
            vkFreeMemory(device, bufferMemory, nullptr);
        }
        buffer = VK_NULL_HANDLE;
        bufferMemory = VK_NULL_HANDLE;
        size = 0;
    }

    void VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &outBuffer, VkDeviceMemory &outMemory)
    {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &outBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, outBuffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &outMemory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        if (vkBindBufferMemory(device, outBuffer, outMemory, 0) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to bind buffer memory!");
        }
    }

    void VulkanBuffer::copyBuffer(VkBuffer src, VkBuffer dst, VkDeviceSize size)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0;
        copyRegion.dstOffset = 0;
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(transferQueue, 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(transferQueue);

        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }

}