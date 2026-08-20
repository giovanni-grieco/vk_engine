#pragma once

#include <vulkan/vulkan.h>
#include <vector>

#include "vk_shader.hpp"
#include "vk_swap_chain.hpp"
#include "vk_pipeline_layout.hpp"
#include "vk_render_pass.hpp"

namespace engine
{
    class VulkanPipeline
    {
    public:
        VkDevice device = VK_NULL_HANDLE;
        VkPipeline pipeline = VK_NULL_HANDLE;
        VkPipelineLayout layout = VK_NULL_HANDLE;
        VulkanPipeline(VulkanDevice &device, VulkanSwapChain &swapChain, VulkanPipelineLayout &pipelineLayout, VulkanRenderPass &renderPass, std::vector<std::string> &shaderFilePaths, std::vector<VulkanShaderType> &types);
        ~VulkanPipeline();
    };
}