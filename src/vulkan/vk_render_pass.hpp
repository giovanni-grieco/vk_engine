#pragma once

#include <vulkan/vulkan.h>
#include "vk_device.hpp"
#include "vk_swap_chain.hpp"

namespace engine{
    class VulkanRenderPass{
        public:
            VkDevice device;
            VkRenderPass renderPass;
            VulkanRenderPass(VulkanDevice& device, VulkanSwapChain& swapChain);
            ~VulkanRenderPass();
    };
}