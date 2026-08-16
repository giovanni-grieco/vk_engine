#pragma once

#include <vulkan/vulkan.h>
#include "vk_command_pool.hpp"
#include "vk_device.hpp"
#include "vk_render_pass.hpp"
#include "vk_framebuffers.hpp"
#include "vk_swap_chain.hpp"
#include "vk_pipeline.hpp"
#include "vk_buffer.hpp"

#include <vector>

namespace engine{
    class VulkanCommandBuffer{
        public:
            std::vector<VkCommandBuffer> commandBuffers;
            VulkanCommandBuffer(int bufferAmount, VulkanDevice& device, VulkanCommandPool& commandPool);

            void recordCommandBuffer(uint32_t imageIndex, uint32_t currentFrame, VulkanPipeline& pipeline, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass, VulkanFramebuffers& frameBuffers, VulkanBuffer& vertexBuffer, uint32_t vertexCount);
    };

}