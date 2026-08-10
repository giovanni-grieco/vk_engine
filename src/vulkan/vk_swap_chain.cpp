#include "vk_swap_chain.hpp"
#include "vk_swap_chain_support_details.hpp"
#include "vk_queue_indices.hpp"

namespace engine
{
    VulkanSwapChain::VulkanSwapChain(VulkanDevice &device, VulkanSurface &surface, Window &window)
    {
        VulkanSwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(device.physicalDevice, surface.surface);

        VkPresentModeKHR presentMode = swapChainSupportDetails.choosePresentMode();
        VkSurfaceFormatKHR surfaceFormat = swapChainSupportDetails.chooseSwapSurfaceFormat();
        VkExtent2D extent = swapChainSupportDetails.chooseSwapExtent(window);

        uint32_t imageCount = swapChainSupportDetails.capabilities.minImageCount + 1;

        if (swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > swapChainSupportDetails.capabilities.maxImageCount)
        {
            imageCount = swapChainSupportDetails.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface.surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        VulkanQueueFamilyIndices indices = findQueueFamilies(device.physicalDevice, surface.surface);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;     // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupportDetails.capabilities.currentTransform;

        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(device.device, &createInfo, nullptr, &swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create swapchain");
        }

        this->device = device.device;
        this->extent = extent;
        this->format = surfaceFormat.format;

        uint32_t imagesCount = 0;
        VkResult result = vkGetSwapchainImagesKHR(device.device, swapChain, &imagesCount, nullptr);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to acquire swapchain images count");
        }
        images.resize(imagesCount);
        vkGetSwapchainImagesKHR(device.device, swapChain, &imagesCount, images.data());
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to acquire swapchain images");
        }
    }

    VulkanSwapChain::~VulkanSwapChain()
    {
        if (swapChain != VK_NULL_HANDLE && device != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(device, swapChain, nullptr);
        }
    }
}