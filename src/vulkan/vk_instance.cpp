#include "vk_instance.hpp"

namespace engine{

    void checkExtensionSupport(uint32_t glfwExtensionCount, uint32_t vkExtensionCount, const char **glfwExtensions, std::vector<VkExtensionProperties> &vkExtensions)
    {
        std::cout << "------------------\n";

        std::map<int, bool> supportedExtensions{};

        for (int i = 0; i < glfwExtensionCount; i++)
        {
            bool foundExtension = false;
            for (int j = 0; j < vkExtensionCount && !foundExtension; j++)
            {
                {
                    foundExtension = (strcmp(glfwExtensions[i], vkExtensions[j].extensionName) == 0);
                }
            }
            supportedExtensions.emplace(i, foundExtension);
        }

        for (auto &kv : supportedExtensions)
        {
            auto index = kv.first;
            auto supported = kv.second;

            std::string supportString = "";

            if (supported)
                supportString = " is SUPPORTED";
            else
                supportString = " is NOT SUPPORTED";

            std::cout << vkExtensions[index].extensionName << supportString << "\n";
        }

        std::cout << "------------------\n";
    }

    VulkanInstance::VulkanInstance(std::string applicationName, Window& window, bool enableValidationLayers, const std::vector<const char*>& validationLayers){
        std::cout << "Enable validation layers: \t " << enableValidationLayers << "\n";
        std::cout << "Check validation layer support:  " << checkValidationLayerSupport(validationLayers) << "\n";
        if (enableValidationLayers && !checkValidationLayerSupport(validationLayers))
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = applicationName.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "vk_engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        std::vector<const char *> requiredExtensions = window.getRequiredExtensions();

        for (int i = 0; i < requiredExtensions.size(); i++)
        {
            std::cout << "\t" << requiredExtensions[i] << "\n";
        }

        createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
        createInfo.ppEnabledExtensionNames = requiredExtensions.data();

        createInfo.enabledLayerCount = 0;
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance!");
        }

        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::cout << "Extensions available: " << extensionCount << "\n";
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        for (const VkExtensionProperties &extension : extensions)
        {
            std::cout << "\t" << extension.extensionName << "\n";
        }

        checkExtensionSupport(requiredExtensions.size(), extensionCount, requiredExtensions.data(), extensions);
    }

    VulkanInstance::~VulkanInstance(){
        vkDestroyInstance(instance, nullptr);
    }

    bool VulkanInstance::checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        for (const char *layerName : validationLayers)
        {
            bool layerFound = false;

            for (const auto &layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }
}