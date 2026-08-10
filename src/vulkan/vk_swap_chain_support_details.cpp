#include "vk_swap_chain_support_details.hpp"
#include "platform/window.hpp"

#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp


namespace engine{

    VkSurfaceFormatKHR VulkanSwapChainSupportDetails::chooseSwapSurfaceFormat(){
        for (const auto& format : this->formats){
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR){
                return format;
            }
        }
        return formats[0];
    }

    VkPresentModeKHR VulkanSwapChainSupportDetails::choosePresentMode(){
        for (const auto& availableMode : this->presentModes){
            if(availableMode==VK_PRESENT_MODE_MAILBOX_KHR){
                return availableMode;
            }
        }
        //default garantueed to be always available
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D VulkanSwapChainSupportDetails::chooseSwapExtent(Window& window){
        if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()){
            return capabilities.currentExtent;
        }else{
            BufferSize bufferSize = window.getBufferSize();

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(bufferSize.width),
                static_cast<uint32_t>(bufferSize.height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            return actualExtent;
        }
    }

    VulkanSwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface){
        VulkanSwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if(formatCount != 0){
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if(presentModeCount != 0){
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }


        return details;
    }
}