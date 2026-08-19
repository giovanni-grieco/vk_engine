#include "vk_pipeline_layout.hpp"
#include "mesh/vk_push_constant.hpp"

namespace engine{
    VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice& device, VulkanDescriptorSetLayout& descriptorSetLayout){
        this->device = device.device;

        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(ModelPushConstant);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 1;            // Optional
        pipelineLayoutInfo.pSetLayouts = &descriptorSetLayout.layout;         // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 1;    // Optional
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange; // Optional

        if (vkCreatePipelineLayout(device.device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    VulkanPipelineLayout::~VulkanPipelineLayout(){
        if(device != VK_NULL_HANDLE && pipelineLayout != VK_NULL_HANDLE){
            vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
            device = VK_NULL_HANDLE;
            pipelineLayout = VK_NULL_HANDLE;
        }
    }
}