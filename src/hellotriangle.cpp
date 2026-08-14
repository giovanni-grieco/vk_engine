#include "platform/window.hpp"
#include "vulkan/vk_instance.hpp"
#include "vulkan/vk_device.hpp"
#include "vulkan/vk_surface.hpp"
#include "vulkan/vk_queues.hpp"
#include "vulkan/vk_swap_chain.hpp"
#include "vulkan/vk_render_pass.hpp"
#include "vulkan/vk_pipeline_layout.hpp"
#include "vulkan/vk_pipeline.hpp"
#include "vulkan/vk_shader.hpp"
#include "vulkan/vk_framebuffers.hpp"
#include "vulkan/vk_command_pool.hpp"
#include "vulkan/vk_command_buffer.hpp"
#include "vulkan/vk_sync.hpp"

class HelloTriangleApplication
{
public:
    HelloTriangleApplication() : window(WIDTH, HEIGHT, applicationName),
                                 instance(applicationName, window, enableValidationLayers, validationLayers),
                                 surface(instance, window),
                                 device(instance, surface, deviceExtensions),
                                 queues(device, surface),
                                 swapChain(device, surface, window),
                                 renderPass(device, swapChain),
                                 pipelineLayout(device),
                                 pipeline(device, swapChain, pipelineLayout, renderPass, shadersFilePaths, shaderTypes),
                                 frameBuffers(device, swapChain, renderPass),
                                 commandPool(device, surface),
                                 commandBuffer(MAX_FRAMES_IN_FLIGHT, device, commandPool),
                                 sync(MAX_FRAMES_IN_FLIGHT, device)
    {
    }

    void run()
    {
        mainLoop();
    }

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const std::string applicationName = "Hello Triangle";

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    const int MAX_FRAMES_IN_FLIGHT = 2;
    uint32_t currentFrame = 0;

    std::vector<std::string> shadersFilePaths = {"shaders/triangle.vert.spv", "shaders/triangle.frag.spv"};
    std::vector<engine::VulkanShaderType> shaderTypes = {engine::VulkanShaderType::VERTEX, engine::VulkanShaderType::FRAGMENT};

    engine::Window window;
    engine::VulkanInstance instance;
    engine::VulkanSurface surface;
    engine::VulkanDevice device;
    engine::VulkanQueues queues;
    engine::VulkanSwapChain swapChain;
    engine::VulkanRenderPass renderPass;
    engine::VulkanPipelineLayout pipelineLayout;
    engine::VulkanPipeline pipeline;
    engine::VulkanFramebuffers frameBuffers;
    engine::VulkanCommandPool commandPool;
    engine::VulkanCommandBuffer commandBuffer;
    engine::VulkanSyncObjects sync;

    void drawFrame()
    {

        vkWaitForFences(device.device, 1, &sync.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(device.device, 1, &sync.inFlightFences[currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, sync.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);
        
        vkResetCommandBuffer(commandBuffer.commandBuffers[currentFrame], 0);
        commandBuffer.recordCommandBuffer(imageIndex, currentFrame, pipeline, swapChain, renderPass, frameBuffers);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {sync.imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer.commandBuffers[currentFrame];

        VkSemaphore signalSemaphores[] = {sync.renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(queues.graphicsQueue, 1, &submitInfo, sync.inFlightFences[currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to submit draw command buffer");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {swapChain.swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(queues.presentQueue, &presentInfo);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void mainLoop()
    {
        // throw std::runtime_error("Failed to run main loop");
        while (!glfwWindowShouldClose(window.window))
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device.device);
    }
};

int main()
{
    HelloTriangleApplication app;

    try
    {
        app.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}