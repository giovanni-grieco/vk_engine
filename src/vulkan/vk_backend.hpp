#pragma once

#include "platform/window.hpp"
#include "rendering/frame_info.hpp"
#include "rendering/draw_packet.hpp"
#include "rendering/light_info.hpp"

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
#include "vk_depth_buffer.hpp"
#include "vk_command_pool.hpp"
#include "vk_command_buffer.hpp"
#include "vk_sync.hpp"
#include "vk_mesh_buffer_manager.hpp"
#include "vk_texture_manager.hpp"
#include "vk_descriptor_set_layout.hpp"
#include "vk_descriptor_pool.hpp"
#include "vk_descriptor_sets.hpp"
#include "vk_host_buffer.hpp"

#include "texture/texture.hpp"
#include "data/vk_uniform_buffer_object.hpp"
#include "data/vk_point_light.hpp"

#include <vector>

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
        VulkanDepthBuffer depthBuffer;
        VulkanRenderPass renderPass;
        VulkanDescriptorSetLayout descriptorSetLayout;
        VulkanPipelineLayout pipelineLayout;
        VulkanPipeline pipeline;
        VulkanFramebuffers frameBuffers;
        VulkanCommandPool commandPool;
        VulkanCommandBuffer commandBuffer;
        VulkanSyncObjects sync;
        VulkanBufferManager bufferManager;
        MeshID meshTriangle1;
        MeshID meshTriangle2;
        MeshID meshTriangle3;
        MeshID meshQuad;
        VulkanTextureManager textureManager;
        TextureID defaultTextureId = -1;
        VulkanHostBuffer uniformBuffer;
        VulkanHostBuffer lightBuffer;
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

        // void drawFrame(Window &window);
        void drawFrame(Window &window, FrameInfo &frameInfo);
        void waitForIdle();

        // Top-down API: register CPU mesh data and get a handle back.
        MeshID addMesh(const Mesh &mesh);
        
        // Top-down API: mark a mesh handle as deleted (leaves a GPU hole
        // until compactBuffers() is called).
        void removeMesh(MeshID mesh);

        // Top-down API: rebuild the unified buffers with only live meshes.
        // Stalls the device; call when no frames are in flight.
        // Top-down API: upload a CPU texture and get a handle back.
        TextureID addTexture(const Texture &texture);

        // Top-down API: mark a texture handle as deleted (its GPU image is
        // destroyed when compactTextures() is called).
        void removeTexture(TextureID texture);

        // Top-down API: destroy the GPU images of removed textures.
        // Stalls the device; call when no frames are in flight.
        void compactTextures();

        void compactBuffers();



        // Top-down API: queue the draw packets for the next frame.
        void submitDrawPackets(const std::vector<DrawPacket> &drawPackets);

        // Top-down API: queue the point-light list for the next frame.
        void submitLights(const LightInfo &lightInfo);


        void recreateSwapChain(Window &window);

    private:
        std::vector<DrawPacket> drawPackets_;
        LightBufferData lights_;


        void updateUbo(uint32_t currentFrameIndex, UniformBufferObject &ubo);
    };
}