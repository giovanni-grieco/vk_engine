#include "vk_command_pool.hpp"
#include "vk_queue_indices.hpp"

namespace engine{
    VulkanCommandPool::VulkanCommandPool(VulkanDevice& device, VulkanSurface& surface){
        this->device = device.device;
        
        VulkanQueueFamilyIndices queueFamilyIndices = findQueueFamilies(device.physicalDevice, surface.surface);
        
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if(vkCreateCommandPool(device.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS){
            throw std::runtime_error("failed to create command pool!");
        }
    }

    VulkanCommandPool::~VulkanCommandPool(){
        vkDestroyCommandPool(device, commandPool, nullptr);
    }
}