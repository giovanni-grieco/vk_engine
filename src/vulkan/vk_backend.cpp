#include "vk_backend.hpp"

#include "geometry/vertex.hpp"
#include "geometry/mesh.hpp"
#include "texture/texture.hpp"
#include "data/vk_uniform_buffer_object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <exception>

namespace engine
{
    namespace
    {
        // 1x1 white texture bound for untextured draws. Vulkan requires a
        // descriptor set at (set = 1) for this pipeline, so we bind this dummy
        // instead of a real texture; the shader never samples it (hasTexture=0).
        Texture makeWhiteTexture()
        {
            Texture texture{};
            texture.texWidth = 1;
            texture.texHeight = 1;
            texture.texChannels = 4;
            texture.pixels = {255, 255, 255, 255};
            return texture;
        }
    }

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
          depthBuffer(device, swapChain.extent.width, swapChain.extent.height),
          renderPass(device, swapChain, depthBuffer.depthFormat()),
          descriptorSetLayout(device.device),
          pipelineLayout(device, descriptorSetLayout),
          pipeline(device, swapChain, pipelineLayout, renderPass, shadersFilePaths, shaderTypes),
          frameBuffers(device, swapChain, renderPass, depthBuffer),
          commandPool(device, surface),
          commandBuffer(MAX_FRAMES_IN_FLIGHT, device, commandPool),
          sync(MAX_FRAMES_IN_FLIGHT, swapChain.images.size(), device),
          bufferManager(device, commandPool, queues),
          textureManager(device, commandPool, queues, descriptorSetLayout.textureSetLayout),
          uniformBuffer(device, sizeof(UniformBufferObject), MAX_FRAMES_IN_FLIGHT),
          lightBuffer(device, sizeof(LightBufferData), MAX_FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT),
          descriptorPool(device.device, MAX_FRAMES_IN_FLIGHT),
          descriptorSets(device, descriptorSetLayout, descriptorPool, uniformBuffer, lightBuffer, MAX_FRAMES_IN_FLIGHT)
    {
        Mesh triangle1Mesh{triangle1, {0, 1, 2}};
        Mesh triangle2Mesh{triangle2, {0, 1, 2}};
        Mesh triangle3Mesh{triangle3, {0, 1, 2}};
        Mesh quadMesh{quadVertices, quadIndices};

        meshTriangle1 = bufferManager.addMesh(triangle1Mesh);
        meshTriangle2 = bufferManager.addMesh(triangle2Mesh);
        meshTriangle3 = bufferManager.addMesh(triangle3Mesh);
        meshQuad = bufferManager.addMesh(quadMesh);
        
        whiteTextureId = textureManager.addTexture(makeWhiteTexture());
    }

    MeshID VulkanBackend::addMesh(const Mesh &mesh)
    {
        return bufferManager.addMesh(mesh);
    }

    uint32_t VulkanBackend::getSubMeshCount(MeshID meshId) const
    {
        const uint32_t count = bufferManager.getSubMeshCount(meshId);
        return count == 0 ? 1 : count;
    }

    SubMesh VulkanBackend::getSubMesh(MeshID meshId, uint32_t subMeshIndex) const
    {
        return bufferManager.getSubMesh(meshId, subMeshIndex);
    }

    void VulkanBackend::submitDrawPackets(const std::vector<DrawPacket> &drawPackets)
    {
        this->drawPackets_ = drawPackets;
    }

    void VulkanBackend::submitLights(const LightInfo &lightInfo)
    {
        LightBufferData lightBufferData{};
        lightBufferData.ambient.colorAndIntensity = {lightInfo.ambient.color, lightInfo.ambient.intensity};
        lightBufferData.directional.directionAndIntensity = {lightInfo.directional.direction, lightInfo.directional.intensity};
        lightBufferData.directional.colorAndRadius = {lightInfo.directional.color, 1};
        
        for(int i = 0; i<lightInfo.pointLights.size() && i < MAX_POINT_LIGHTS_VK; i++){
            lightBufferData.lightCount++;
            lightBufferData.lights[i].positionAndIntensity = {lightInfo.pointLights[i].position, lightInfo.pointLights[i].intensity};
            lightBufferData.lights[i].colorAndRadius = {lightInfo.pointLights[i].color, 1};
        }
        this->lights_ = lightBufferData;
    }

    void VulkanBackend::removeMesh(MeshID mesh)
    {
        bufferManager.removeMesh(mesh);
    }

    void VulkanBackend::compactBuffers()
    {
        bufferManager.compact();
    }

    TextureID VulkanBackend::addTexture(const Texture &texture)
    {
        return textureManager.addTexture(texture);
    }

    std::vector<TextureID> VulkanBackend::addSubMeshTextures(const Mesh &mesh, TextureID fallback)
    {
        std::vector<TextureID> handles;
        handles.reserve(mesh.subMeshes.empty() ? 1 : mesh.subMeshes.size());

        for (const SubMesh &sub : mesh.subMeshes)
        {
            TextureID t = fallback;
            if (!sub.diffuseTexturePath.empty())
            {
                try
                {
                    t = textureManager.addTexture(createTextureFromFile(sub.diffuseTexturePath));
                }
                catch (const std::exception &)
                {
                    t = fallback;
                }
            }
            handles.push_back(t);
        }

        // Procedural meshes have no submeshes; still provide one texture slot.
        if (handles.empty())
        {
            handles.push_back(fallback);
        }
        return handles;
    }

    void VulkanBackend::removeTexture(TextureID texture)
    {
        textureManager.removeTexture(texture);
    }

    void VulkanBackend::compactTextures()
    {
        textureManager.compact();
    }

    void VulkanBackend::updateUbo(uint32_t currentFrameIndex, UniformBufferObject &ubo)
    {
        uniformBuffer.update(currentFrameIndex, &ubo, sizeof(UniformBufferObject));
    }

    void VulkanBackend::drawFrame(Window &window, FrameInfo &frameInfo)
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
        // updateUbo(currentFrame);

        UniformBufferObject ubo{frameInfo.view, frameInfo.projection};
        
        uniformBuffer.update(currentFrame, &ubo, sizeof(UniformBufferObject));
        lightBuffer.update(currentFrame, &lights_, sizeof(LightBufferData));

        // Use the application's draw packets if any were submitted; otherwise
        // render the default demo scene.
        std::vector<DrawPacket> drawPackets = drawPackets_;
        if (drawPackets.empty())
        {
            static auto startTime = std::chrono::high_resolution_clock::now();
            auto currentTime = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
            glm::mat4 spin = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            drawPackets = {
                DrawPacket{.meshId = meshTriangle1, .textureId = -1, .model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f))},
                DrawPacket{.meshId = meshTriangle2, .textureId = -1, .model = spin},
                DrawPacket{.meshId = meshTriangle3, .textureId = -1, .model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f))},
                DrawPacket{.meshId = meshQuad, .textureId = -1, .model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f))},
            };
        }

        commandBuffer.recordCommandBuffer(
            imageIndex,
            currentFrame,
            pipeline,
            swapChain,
            renderPass,
            frameBuffers,
            bufferManager,
            textureManager,
            whiteTextureId,
            drawPackets,
            descriptorSets);

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
        depthBuffer.recreate(device, swapChain.extent.width, swapChain.extent.height);
        frameBuffers.recreate(device, swapChain, renderPass, depthBuffer);
    }
}