#pragma once

#include <vulkan/vulkan.h>
#include <stdexcept>

#include "vk_descriptor_set_layout.hpp"

namespace engine{
    class VulkanDescriptorSetLayout{
        public:
            VkDevice device = VK_NULL_HANDLE;
            VkDescriptorSetLayout layout = VK_NULL_HANDLE;

            VulkanDescriptorSetLayout(VkDevice device);
            ~VulkanDescriptorSetLayout();
        private:
            void init(VkDevice device);
            void destroy();
    };
}