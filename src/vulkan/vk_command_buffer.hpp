#pragma once

#include <vulkan/vulkan.h>
#include "vk_command_pool.hpp"
#include "vk_device.hpp"
#include "vk_render_pass.hpp"
#include "vk_framebuffers.hpp"
#include "vk_swap_chain.hpp"
#include "vk_pipeline.hpp"
#include "vk_buffer.hpp"
#include "vk_mesh_buffer_manager.hpp"
#include "vk_texture_manager.hpp"
#include "vk_descriptor_sets.hpp"
#include "rendering/draw_packet.hpp"

#include <vector>

namespace engine
{
    class VulkanCommandBuffer
    {
    public:
        std::vector<VkCommandBuffer> commandBuffers;
        VulkanCommandBuffer(int bufferAmount, VulkanDevice &device, VulkanCommandPool &commandPool);

        void recordCommandBuffer(
            uint32_t imageIndex,
            uint32_t currentFrame,
            VulkanPipeline &pipeline,
            VulkanSwapChain &swapChain,
            VulkanRenderPass &renderPass,
            VulkanFramebuffers &frameBuffers,
            VulkanBuffer &vertexBuffer,
            uint32_t vertexCount,
            VulkanDescriptorSets &descriptorSets);

        void recordCommandBuffer(
            uint32_t imageIndex,
            uint32_t currentFrame,
            VulkanPipeline &pipeline,
            VulkanSwapChain &swapChain,
            VulkanRenderPass &renderPass,
            VulkanFramebuffers &frameBuffers,
            VulkanBuffer &vertexBuffer,
            VulkanBuffer &indexBuffer,
            uint32_t indexSize,
            VulkanDescriptorSets &descriptorSets);

        // Batched draw: binds the unified buffers and UBO set once, then issues
        // one vkCmdDrawIndexed per DrawPacket — pushing its model matrix and
        // binding the descriptor set of its texture.
        void recordCommandBuffer(
            uint32_t imageIndex,
            uint32_t currentFrame,
            VulkanPipeline &pipeline,
            VulkanSwapChain &swapChain,
            VulkanRenderPass &renderPass,
            VulkanFramebuffers &frameBuffers,
            VulkanBufferManager &bufferManager,
            VulkanTextureManager &textureManager,
            TextureID whiteTextureId,
            const std::vector<DrawPacket> &drawPackets,
            VulkanDescriptorSets &descriptorSets);
    };

}