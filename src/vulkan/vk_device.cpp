#include "vk_device.hpp"

#include "vk_swap_chain_support_details.hpp"

namespace engine{

    VulkanDevice::VulkanDevice(VulkanInstance& instance, VulkanSurface& surface, const std::vector<const char *>& deviceExtensions, bool enableValidationLayers, const std::vector<const char *> &validationLayers){
        
        pickPhysicalDevice(instance.instance, surface.surface, deviceExtensions);
        
        engine::VulkanQueueFamilyIndices indices = engine::findQueueFamilies(physicalDevice, surface.surface);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create logical device!");
        }
    }

    VulkanDevice::~VulkanDevice(){
        if (device != VK_NULL_HANDLE) {
            vkDestroyDevice(device, nullptr);
        }
    }

    bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& deviceExtensions)
    {
        uint32_t deviceExtensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, nullptr);
        std::vector<VkExtensionProperties> availableDeviceExtensions(deviceExtensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &deviceExtensionCount, availableDeviceExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());
        std::cout << "Checking:\n";
        for (const auto& availableExtension : availableDeviceExtensions){
            if(requiredExtensions.find(availableExtension.extensionName) != requiredExtensions.end()){
                std::cout << "\t" << availableExtension.extensionName << " is supported!\n";
            }
            requiredExtensions.erase(availableExtension.extensionName);
        }

        return requiredExtensions.empty();
    }

    bool checkSwapChainSupportDetails(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface){
        VulkanSwapChainSupportDetails swapChainSupportDetails = querySwapChainSupport(physicalDevice, surface);
        //both of them must not be empty
        return !(swapChainSupportDetails.formats.empty() && swapChainSupportDetails.presentModes.empty());
    }

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, const std::vector<const char *>& deviceExtensions)
    {
        // TODO
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);

        bool result = engine::findQueueFamilies(physicalDevice, surface).isComplete() 
        && checkDeviceExtensionSupport(physicalDevice, deviceExtensions)
        && checkSwapChainSupportDetails(physicalDevice, surface);
        if (!result)
        {
            std::cout << "GPU " << deviceProperties.deviceName << " is not suitable!\n";
        }
        else
        {
            std::cout << "GPU " << deviceProperties.deviceName << " is suitable!\n";
        }
        return result;
    }

    void VulkanDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface, const std::vector<const char *>& deviceExtensions)
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto &phyDev : devices)
        {
            if (isDeviceSuitable(phyDev, surface, deviceExtensions))
            {
                physicalDevice = phyDev;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find suitable GPU!");
        }
    }
}