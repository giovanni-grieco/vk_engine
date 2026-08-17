#pragma once

#include "platform/window.hpp"

#include "vk_instance.hpp"
#include "vk_device.hpp"
#include "vk_surface.hpp"
#include "vk_queues.hpp"
#include "vk_swap_chain.hpp"
#include "vk_render_pass.hpp"
#include "vk_pipeline_layout.hpp"
#include "vk_pipeline.hpp"
#include "vk_shader.hpp"
#include "vk_framebuffers.hpp"
#include "vk_command_pool.hpp"
#include "vk_command_buffer.hpp"
#include "vk_sync.hpp"
#include "vk_buffer.hpp"
#include "vk_descriptor_set_layout.hpp"
#include "vk_descriptor_pool.hpp"  
#include "vk_descriptor_sets.hpp"   
#include "vk_uniform_buffer.hpp"


namespace engine
{
    class VulkanBackend
    {
        public:
            const uint32_t MAX_FRAMES_IN_FLIGHT = 2;

            uint32_t currentFrame = 0;

            VulkanInstance instance;
            VulkanSurface surface;
            VulkanDevice device;
            VulkanQueues queues;
            VulkanSwapChain swapChain;
            VulkanRenderPass renderPass;
            VulkanDescriptorSetLayout descriptorSetLayout;
            VulkanPipelineLayout pipelineLayout;
            VulkanPipeline pipeline;
            VulkanFramebuffers frameBuffers;
            VulkanCommandPool commandPool;
            VulkanCommandBuffer commandBuffer;
            VulkanSyncObjects sync;
            VulkanBuffer vertexBuffer1;
            VulkanBuffer vertexBuffer2;
            VulkanBuffer vertexBuffer3;
            VulkanBuffer vertexBufferQuad;
            VulkanBuffer indexBufferQuad;
            VulkanUniformBuffer uniformBuffer;
            VulkanDescriptorPool descriptorPool;
            VulkanDescriptorSets descriptorSets;
            
            VulkanBackend(std::string applicationName,
                        bool enableValidationLayers,
                        const std::vector<const char *> validationLayers,
                        const std::vector<const char *> deviceExtensions,
                        std::vector<std::string> shadersFilePaths,
                        std::vector<engine::VulkanShaderType> shaderTypes,
                        Window &window);

            VulkanBackend(const VulkanBackend &other) = delete;
            VulkanBackend &operator=(const VulkanBackend &other) = delete;

            VulkanBackend(VulkanBackend &&other) = delete;
            VulkanBackend &operator=(VulkanBackend &&other) = delete;

            void drawFrame(Window &window);
            void waitForIdle();

            void recreateSwapChain(Window &window);

        private:
            int frameCount = 0;
            const int frameSwitch = 10;

            void updateUbo(uint32_t currentFrameIndex);
    };
}