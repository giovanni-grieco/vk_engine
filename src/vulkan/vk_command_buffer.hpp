#pragma once

#include <vulkan/vulkan.h>
#include "vk_command_pool.hpp"
#include "vk_device.hpp"
#include "vk_render_pass.hpp"
#include "vk_framebuffers.hpp"
#include "vk_swap_chain.hpp"
#include "vk_pipeline.hpp"

namespace engine{
    class VulkanCommandBuffer{
        public:
            VkCommandBuffer commandBuffer;
            VulkanCommandBuffer(VulkanDevice& device, VulkanCommandPool& commandPool);

            void recordCommandBuffer(uint32_t imageIndex, VulkanPipeline& pipeline, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass, VulkanFramebuffers& frameBuffers);
    };

}