#include "vk_queues.hpp"

namespace engine
{

    VulkanQueues::VulkanQueues(VulkanDevice &device, VulkanSurface &surface)
    {
        VulkanQueueFamilyIndices indices = findQueueFamilies(device.physicalDevice, surface.surface);
        vkGetDeviceQueue(device.device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device.device, indices.presentFamily.value(), 0, &presentQueue);
    }
}