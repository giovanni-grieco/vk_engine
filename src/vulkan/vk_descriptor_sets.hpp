#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "vk_device.hpp"
#include "vk_descriptor_set_layout.hpp"
#include "vk_descriptor_pool.hpp"
#include "vk_uniform_buffer.hpp"

namespace engine
{
    class VulkanDescriptorSets
    {
    public:
        std::vector<VkDescriptorSet> descriptorSets;

        VulkanDescriptorSets(VulkanDevice &device,
                             VulkanDescriptorSetLayout &layout,
                             VulkanDescriptorPool &pool,
                             VulkanUniformBuffer &uniformBuffer,
                             VulkanUniformBuffer &lightBuffer,
                             uint32_t framesInFlight);

        VulkanDescriptorSets(const VulkanDescriptorSets &) = delete;
        VulkanDescriptorSets &operator=(const VulkanDescriptorSets &) = delete;
    };
}