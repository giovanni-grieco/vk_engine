#pragma once

#include <vulkan/vulkan.h>
#include "vk_device.hpp"

namespace engine{
    class VulkanPipelineLayout{
        public:
            VkDevice device;
            VkPipelineLayout pipelineLayout;
            VulkanPipelineLayout(VulkanDevice& device);
            ~VulkanPipelineLayout();
    };
}