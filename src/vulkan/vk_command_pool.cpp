#include "vk_command_pool.hpp"
#include "vk_queue_indices.hpp"

namespace engine
{
    VulkanCommandPool::VulkanCommandPool(VulkanDevice &device, VulkanSurface &surface)
    {
        this->device = device.device;

        VulkanQueueFamilyIndices queueFamilyIndices = findQueueFamilies(device.physicalDevice, surface.surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create command pool!");
        }
    }

    VulkanCommandPool::~VulkanCommandPool()
    {
        destroy();
    }

    VulkanCommandPool::VulkanCommandPool(VulkanCommandPool &&other) noexcept
    {
        this->device = other.device;
        this->commandPool = other.commandPool;
        other.device = VK_NULL_HANDLE;
        other.commandPool = VK_NULL_HANDLE;
    }

    VulkanCommandPool &VulkanCommandPool::operator=(VulkanCommandPool &&other) noexcept
    {
        if (this != &other)
        {
            this->destroy();
            this->device = other.device;
            this->commandPool = other.commandPool;
            other.device = VK_NULL_HANDLE;
            other.commandPool = VK_NULL_HANDLE;
        }
        return *this;
    }

    void VulkanCommandPool::destroy()
    {
        if (device != VK_NULL_HANDLE && commandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(device, commandPool, nullptr);
        }
        device = VK_NULL_HANDLE;
        commandPool = VK_NULL_HANDLE;
    }
}