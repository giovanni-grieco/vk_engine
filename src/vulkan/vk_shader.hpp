#pragma once

#include <vulkan/vulkan.h>
#include <vector>
#include "vulkan/vk_device.hpp"

namespace engine{
    enum VulkanShaderType{
        VERTEX,
        FRAGMENT
    };

    class VulkanShader{
        public:
            VkDevice device;
            VkShaderModule shaderModule;
            VulkanShaderType type;
            VulkanShader(VulkanDevice& device, const std::vector<char>& code, VulkanShaderType type);
            ~VulkanShader();
    };
}