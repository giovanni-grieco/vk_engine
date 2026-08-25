#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include "vk_descriptor_set_layout.hpp"

namespace engine
{
    class VulkanDescriptorSetLayout
    {
    public:
        VkDevice device = VK_NULL_HANDLE;
        VkDescriptorSetLayout uboSetLayout = VK_NULL_HANDLE;     // set 0: per-frame UBO
        VkDescriptorSetLayout textureSetLayout = VK_NULL_HANDLE; // set 1: per-texture sampler

        VulkanDescriptorSetLayout(VkDevice device);
        ~VulkanDescriptorSetLayout();

    private:
        void init(VkDevice device);
        void destroy();
    };
}