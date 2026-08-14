#include "vk_backend.hpp"

namespace engine
{
    VulkanBackend::VulkanBackend(
        std::string applicationName,
        bool enableValidationLayers,
        const std::vector<const char *> validationLayers,
        const std::vector<const char *> deviceExtensions,
        std::vector<std::string> shadersFilePaths,
        std::vector<engine::VulkanShaderType> shaderTypes,
        Window &window)
        : instance(applicationName, window, enableValidationLayers, validationLayers),
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
          sync(MAX_FRAMES_IN_FLIGHT, swapChain.images.size(), device)
    {
    }

    void VulkanBackend::drawFrame(){
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

        VkSemaphore signalSemaphores[] = {sync.renderFinishedSemaphores[imageIndex]};
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

    void VulkanBackend::waitForIdle(){
        vkDeviceWaitIdle(device.device);
    }

    void VulkanBackend::recreateSwapChain(Window& window){
        waitForIdle();

        swapChain.recreate(device, surface, window);
        frameBuffers.recreate(device, swapChain, renderPass);
    }
}