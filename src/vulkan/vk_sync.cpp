#include "vk_sync.hpp"

namespace engine
{

    VulkanSyncObjects::VulkanSyncObjects(size_t framesInFlight, size_t swapchainImageCount, VulkanDevice &device)
    {
        this->device = device.device;
        this->imageAvailableSemaphores.resize(framesInFlight);
        this->inFlightFences.resize(framesInFlight);
        // One render-finished semaphore per swapchain image: a semaphore used in a
        // present operation can only be reused once that image has been re-acquired.
        this->renderFinishedSemaphores.resize(swapchainImageCount);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < framesInFlight; i++)
        {
            if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(device.device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create semaphores/fences!");
            }
        }

        for (size_t i = 0; i < renderFinishedSemaphores.size(); i++)
        {
            if (vkCreateSemaphore(device.device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create render finished semaphores!");
            }
        }
    }

    VulkanSyncObjects::~VulkanSyncObjects()
    {
        for (size_t i = 0; i<imageAvailableSemaphores.size(); i++){
            vkDestroySemaphore(device, imageAvailableSemaphores[i], nullptr);
        }
        for (size_t i = 0; i<renderFinishedSemaphores.size(); i++){
            vkDestroySemaphore(device, renderFinishedSemaphores[i], nullptr);
        }
        for (size_t i = 0; i<inFlightFences.size(); i++){
            vkDestroyFence(device, inFlightFences[i], nullptr);
        }
    }
}