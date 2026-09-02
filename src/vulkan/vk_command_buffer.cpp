#include "vk_command_buffer.hpp"

#include "geometry/vertex.hpp"
#include "data/vk_uniform_buffer_object.hpp"
#include "data/vk_push_constant.hpp"

namespace engine
{
    VulkanCommandBuffer::VulkanCommandBuffer(int bufferAmount, VulkanDevice &device, VulkanCommandPool &commandPool)
    {
        this->commandBuffers.resize(bufferAmount);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool.commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(bufferAmount);

        if (vkAllocateCommandBuffers(device.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void VulkanCommandBuffer::recordCommandBuffer(
        uint32_t imageIndex,
        uint32_t currentFrame,
        VulkanPipeline &pipeline,
        VulkanSwapChain &swapChain,
        VulkanRenderPass &renderPass,
        VulkanFramebuffers &frameBuffers,
        VulkanBuffer &vertexBuffer,
        uint32_t vertexCount,
        VulkanDescriptorSets &descriptorSets)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = frameBuffers.frameBuffers[imageIndex];

        renderPassInfo.renderArea = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.extent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

        VkDescriptorSet sets[] = {descriptorSets.descriptorSets[currentFrame]};
        vkCmdBindDescriptorSets(commandBuffers[currentFrame],
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.layout,
                                0, 1, sets,
                                0, nullptr);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.extent.width);
        viewport.height = static_cast<float>(swapChain.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.extent;
        vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

        vkCmdDraw(commandBuffers[currentFrame], vertexCount, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);

        if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void VulkanCommandBuffer::recordCommandBuffer(
        uint32_t imageIndex,
        uint32_t currentFrame,
        VulkanPipeline &pipeline,
        VulkanSwapChain &swapChain,
        VulkanRenderPass &renderPass,
        VulkanFramebuffers &frameBuffers,
        VulkanBuffer &vertexBuffer,
        VulkanBuffer &indexBuffer,
        uint32_t indexSize,
        VulkanDescriptorSets &descriptorSets)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffers[currentFrame], &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = frameBuffers.frameBuffers[imageIndex];

        renderPassInfo.renderArea = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.extent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[currentFrame], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[currentFrame], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

        VkBuffer vertexBuffers[] = {vertexBuffer.buffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffers[currentFrame], 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(commandBuffers[currentFrame], indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

        VkDescriptorSet sets[] = {descriptorSets.descriptorSets[currentFrame]};
        vkCmdBindDescriptorSets(commandBuffers[currentFrame],
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.layout,
                                0, 1, sets,
                                0, nullptr);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.extent.width);
        viewport.height = static_cast<float>(swapChain.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffers[currentFrame], 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.extent;
        vkCmdSetScissor(commandBuffers[currentFrame], 0, 1, &scissor);

        vkCmdDrawIndexed(commandBuffers[currentFrame], indexSize, 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[currentFrame]);

        if (vkEndCommandBuffer(commandBuffers[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }

    void VulkanCommandBuffer::recordCommandBuffer(
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
        VulkanDescriptorSets &descriptorSets)
    {
        VkCommandBuffer commandBuffer = commandBuffers[currentFrame];

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;
        beginInfo.pInheritanceInfo = nullptr;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.renderPass;
        renderPassInfo.framebuffer = frameBuffers.frameBuffers[imageIndex];

        renderPassInfo.renderArea = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.extent;

        VkClearValue clearValues[2]{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};

        renderPassInfo.clearValueCount = 2;
        renderPassInfo.pClearValues = clearValues;

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);

        // Bind the unified vertex + index buffers once for the whole batch.
        VkBuffer vertexBuffers[] = {bufferManager.vertexBufferHandle()};
        VkDeviceSize vertexOffsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, vertexOffsets);
        vkCmdBindIndexBuffer(commandBuffer, bufferManager.indexBufferHandle(), 0, VK_INDEX_TYPE_UINT32);

        // Set 0 (per frame): the camera uniform buffer, shared by all draws.
        VkDescriptorSet uboSet = descriptorSets.descriptorSets[currentFrame];
        vkCmdBindDescriptorSets(commandBuffer,
                                VK_PIPELINE_BIND_POINT_GRAPHICS,
                                pipeline.layout,
                                0, 1, &uboSet,
                                0, nullptr);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChain.extent.width);
        viewport.height = static_cast<float>(swapChain.extent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.extent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        for (const DrawPacket &packet : drawPackets)
        {
            const MeshDrawInfo *info = bufferManager.getDrawInfo(packet.meshId, packet.subMeshIndex);
            if (info == nullptr)
            {
                continue; // unknown mesh handle or submesh index — skip it
            }

            // Set 1 (per draw): the texture requested by this packet. When the
            // id is -1 or unknown we bind a 1x1 white dummy texture to satisfy
            // the pipeline, but flag the shader to skip sampling (hasTexture=0)
            // so the vertex color is used instead.
            const TextureDrawInfo *texInfo = textureManager.getTextureInfo(packet.textureId);
            const bool hasTexture = (texInfo != nullptr);
            if (texInfo == nullptr)
            {
                texInfo = textureManager.getTextureInfo(whiteTextureId);
            }
            if (texInfo == nullptr)
            {
                continue; // no usable descriptor set — skip it
            }

            VkDescriptorSet textureSet = texInfo->descriptorSet;
            vkCmdBindDescriptorSets(commandBuffer,
                                    VK_PIPELINE_BIND_POINT_GRAPHICS,
                                    pipeline.layout,
                                    1, 1, &textureSet,
                                    0, nullptr);

            ModelPushConstant push{};
            push.model = packet.model;
            push.normal = packet.normal;
            push.color = packet.color;
            push.hasTexture = 0;
            if (hasTexture) {
                push.hasTexture = 1;
            }
            vkCmdPushConstants(commandBuffer,
                               pipeline.layout,
                               VK_SHADER_STAGE_VERTEX_BIT,
                               0,
                               sizeof(ModelPushConstant),
                               &push);

            vkCmdDrawIndexed(commandBuffer,
                             info->indexCount,
                             1,
                             info->firstIndex,
                             info->vertexOffset,
                             0);
        }

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}