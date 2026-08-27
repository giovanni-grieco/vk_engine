#pragma once

#include <vector>
#include <vulkan/vulkan.h>
#include "vk_swap_chain.hpp"
#include "vk_device.hpp"
#include "vk_render_pass.hpp"
#include "vk_depth_buffer.hpp"

namespace engine
{
    class VulkanFramebuffers
    {
    public:
        VkDevice device;
        std::vector<VkFramebuffer> frameBuffers;
        VulkanFramebuffers(VulkanDevice &device, VulkanSwapChain &swapChain, VulkanRenderPass &renderPass, VulkanDepthBuffer &depthBuffer);
        ~VulkanFramebuffers();
        void recreate(VulkanDevice &device, VulkanSwapChain &swapChain, VulkanRenderPass &renderPass, VulkanDepthBuffer &depthBuffer);

    private:
        void init(VulkanDevice &device, VulkanSwapChain &swapChain, VulkanRenderPass &renderPass, VulkanDepthBuffer &depthBuffer);
        void destroy();
    };
}