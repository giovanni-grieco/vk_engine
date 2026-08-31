#include "vk_descriptor_pool.hpp"
#include <stdexcept>

namespace engine
{
    VulkanDescriptorPool::VulkanDescriptorPool(VkDevice device, uint32_t maxSets)
        : device(device)
    {
        VkDescriptorPoolSize poolSizes[2]{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = maxSets;
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        poolSizes[1].descriptorCount = maxSets;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 2;
        poolInfo.pPoolSizes = poolSizes;
        poolInfo.maxSets = maxSets;

        if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
            throw std::runtime_error("failed to create descriptor pool!");
    }

    VulkanDescriptorPool::~VulkanDescriptorPool()
    {
        if (descriptorPool != VK_NULL_HANDLE)
            vkDestroyDescriptorPool(device, descriptorPool, nullptr);
    }
}