#pragma once

#include <vulkan/vulkan.h>

namespace engine
{
    class VulkanDescriptorPool
    {
    public:
        VkDevice device = VK_NULL_HANDLE;
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

        VulkanDescriptorPool(VkDevice device, uint32_t maxSets);
        ~VulkanDescriptorPool();

        VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
        VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;
    };
}