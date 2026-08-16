#include "vk_pipeline.hpp"
#include "vk_pipeline_layout.hpp"
#include "vk_shader.hpp"
#include "util/utils.hpp"
#include "mesh/vk_vertex.hpp"

namespace engine
{

    VulkanPipeline::VulkanPipeline(VulkanDevice &device, VulkanSwapChain &swapChain, VulkanPipelineLayout &pipelineLayout, VulkanRenderPass &renderPass, std::vector<std::string>& shaderFilePaths, std::vector<VulkanShaderType>& types)
    {
        this->device = device.device;
        std::vector<std::vector<char>> shaders;
        std::vector<std::unique_ptr<VulkanShader>> shaderModules;
        for (const auto &shaderFilePath : shaderFilePaths)
        {
            shaders.push_back(readFile(shaderFilePath));
        }

        for (int i = 0; i < shaders.size(); i++)
        {
            auto &shaderCode = shaders[i];
            auto type = types[i];
            shaderModules.push_back(std::make_unique<VulkanShader>(device, shaderCode, type));
        }

        std::vector<VkPipelineShaderStageCreateInfo> shaderStagesCreateInfo{};
        for (int i = 0; i<shaderModules.size() ; i++)
        {
            VkPipelineShaderStageCreateInfo shaderStageInfo{};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            if (types[i] == VulkanShaderType::VERTEX)
            {
                shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            }
            else if (types[i] == VulkanShaderType::FRAGMENT)
            {
                shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            }

            //ERROR ON PURPOSE
            //shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

            shaderStageInfo.module = shaderModules[i]->shaderModule;
            shaderStageInfo.pName = "main";
            shaderStagesCreateInfo.push_back(shaderStageInfo);
        }

        auto bindingDescription = getBindingDescription();
        auto attributeDescription = getAttributeDescription();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescription.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescription.data(); // Optional

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChain.extent.width;
        viewport.height = (float)swapChain.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.extent;

        // OPTIONAL DYNAMIC STATE REQUIRES ViewPort and Scissor to be set at draw time
        std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR};

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates = dynamicStates.data();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        // DISABLED FOR NOW
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f;          // Optional
        multisampling.pSampleMask = nullptr;            // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE;      // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStagesCreateInfo.size());
        pipelineInfo.pStages = shaderStagesCreateInfo.data();

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;

        pipelineInfo.layout = pipelineLayout.pipelineLayout;

        pipelineInfo.renderPass = renderPass.renderPass;

        pipelineInfo.subpass=0;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if(vkCreateGraphicsPipelines(device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS){
            throw std::runtime_error("failed to create graphics pipeline");
        }
    }

    VulkanPipeline::~VulkanPipeline()
    {
        if(device!=VK_NULL_HANDLE && pipeline!=VK_NULL_HANDLE){
            vkDestroyPipeline(this->device, this->pipeline, nullptr);
        }
    }
}