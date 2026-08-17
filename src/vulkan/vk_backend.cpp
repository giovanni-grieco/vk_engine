#include "vk_backend.hpp"

#include "geometry/vertex.hpp"
#include "mesh/vk_uniform_buffer_object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>

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
          descriptorSetLayout(device.device),
          pipelineLayout(device, descriptorSetLayout),
          pipeline(device, swapChain, pipelineLayout, renderPass, shadersFilePaths, shaderTypes),
          frameBuffers(device, swapChain, renderPass),
          commandPool(device, surface),
          commandBuffer(MAX_FRAMES_IN_FLIGHT, device, commandPool),
          sync(MAX_FRAMES_IN_FLIGHT, swapChain.images.size(), device),
          vertexBuffer1(device, commandPool, queues),
          vertexBuffer2(device, commandPool, queues),
          vertexBuffer3(device, commandPool, queues),
          vertexBufferQuad(device, commandPool, queues),
          indexBufferQuad(device, commandPool, queues),
          uniformBuffer(device, sizeof(UniformBufferObject), MAX_FRAMES_IN_FLIGHT),
          descriptorPool(device.device, MAX_FRAMES_IN_FLIGHT),
          descriptorSets(device, descriptorSetLayout, descriptorPool, uniformBuffer, MAX_FRAMES_IN_FLIGHT)
    {
        vertexBuffer1.create(BufferType::VERTEX, sizeof(Vertex) * triangle1.size());
        vertexBuffer1.upload(triangle1.data(), sizeof(Vertex) * triangle1.size());

        vertexBuffer2.create(BufferType::VERTEX, sizeof(Vertex) * triangle2.size());
        vertexBuffer2.upload(triangle2.data(), sizeof(Vertex) * triangle2.size());

        vertexBuffer3.create(BufferType::VERTEX, sizeof(Vertex) * triangle3.size());
        vertexBuffer3.upload(triangle3.data(), sizeof(Vertex) * triangle3.size());

        vertexBufferQuad.create(BufferType::VERTEX, sizeof(Vertex) * quadVertices.size());
        vertexBufferQuad.upload(quadVertices.data(), sizeof(Vertex) * quadVertices.size());

        indexBufferQuad.create(BufferType::INDEX, sizeof(uint16_t) * quadIndices.size());
        indexBufferQuad.upload(quadIndices.data(), sizeof(uint16_t) * quadIndices.size());
    }

    void VulkanBackend::updateUbo(uint32_t currentFrame)
    {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChain.extent.width / (float) swapChain.extent.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        uniformBuffer.update(currentFrame, &ubo, sizeof(UniformBufferObject));
    }

    void VulkanBackend::drawFrame(Window &window)
    {
        vkWaitForFences(device.device, 1, &sync.inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.device, swapChain.swapChain, UINT64_MAX, sync.imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain(window);
            std::cout << "Swap chain recreated!\n";
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to recreated swapchain");
        }
        vkResetFences(device.device, 1, &sync.inFlightFences[currentFrame]);

        vkResetCommandBuffer(commandBuffer.commandBuffers[currentFrame], 0);
        updateUbo(currentFrame);
        if (frameCount <= frameSwitch)
        {
            commandBuffer.recordCommandBuffer(
                imageIndex,
                currentFrame,
                pipeline,
                swapChain,
                renderPass,
                frameBuffers,
                vertexBuffer1,
                triangle1.size(),
                descriptorSets
            );
        }
        else if (frameCount > frameSwitch && frameCount <= frameSwitch * 2)
        {
            commandBuffer.recordCommandBuffer(
                imageIndex,
                currentFrame,
                pipeline,
                swapChain,
                renderPass,
                frameBuffers,
                vertexBuffer2,
                triangle2.size(),
                descriptorSets
            );
        }
        else if (frameCount > frameSwitch * 2 && frameCount <= frameSwitch * 3)
        {
            commandBuffer.recordCommandBuffer(
                imageIndex,
                currentFrame,
                pipeline,
                swapChain,
                renderPass,
                frameBuffers,
                vertexBuffer3,
                triangle3.size(),
                descriptorSets
            );
        }
        else if (frameCount > frameSwitch * 3)
        {
            commandBuffer.recordCommandBuffer(
                imageIndex,
                currentFrame,
                pipeline,
                swapChain,
                renderPass,
                frameBuffers,
                vertexBufferQuad,
                indexBufferQuad,
                static_cast<uint32_t>(quadIndices.size()),
                descriptorSets
            );
        }

        frameCount = (frameCount + 1) % (frameSwitch * 4);

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

        result = vkQueuePresentKHR(queues.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.frameBufferResizeFlag)
        {
            window.frameBufferResizeFlag = false;

            recreateSwapChain(window);
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void VulkanBackend::waitForIdle()
    {
        vkDeviceWaitIdle(device.device);
    }

    void VulkanBackend::recreateSwapChain(Window &window)
    {

        waitForIdle();

        swapChain.recreate(device, surface, window);
        frameBuffers.recreate(device, swapChain, renderPass);
    }
}