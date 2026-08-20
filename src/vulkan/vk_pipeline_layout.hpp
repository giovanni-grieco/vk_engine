#pragma once

#include <vulkan/vulkan.h>
#include "vk_device.hpp"
#include "vk_descriptor_set_layout.hpp"

namespace engine
{
    class VulkanPipelineLayout
    {
    public:
        VkDevice device;
        VkPipelineLayout pipelineLayout;
        VulkanPipelineLayout(VulkanDevice &device, VulkanDescriptorSetLayout &descriptorSetLayout);
        ~VulkanPipelineLayout();

        VulkanPipelineLayout(const VulkanPipelineLayout &other) = delete;
        VulkanPipelineLayout(VulkanPipelineLayout &&other) = delete;

        VulkanPipelineLayout &operator=(const VulkanPipelineLayout &other) = delete;
        VulkanPipelineLayout &operator=(VulkanPipelineLayout &&other) = delete;
    };
}