#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <map>

#include "platform/window.hpp"

#include "vulkan/queue_family.hpp"

using namespace std;

class HelloTriangleApplication
{
public:

    HelloTriangleApplication() : window(WIDTH, HEIGHT, applicationName) {}

    void run()
    {
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    string applicationName = "Hello Triangle";

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    engine::Window window;
    VkInstance instance;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkSurfaceKHR surface;


#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    void createInstance()
    {
        cout << "Enable validation layers: \t " << enableValidationLayers << "\n";
        cout << "Check validation layer support:  " << checkValidationLayerSupport() << "\n";
        if (enableValidationLayers && !checkValidationLayerSupport())
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

        vector<const char *> requiredExtensions = window.getRequiredExtensions();

        for (int i = 0; i < requiredExtensions.size(); i++)
        {
            cout << "\t" << requiredExtensions[i] << "\n";
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
        cout << "Extensions available: " << extensionCount << "\n";
        vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        for (const VkExtensionProperties &extension : extensions)
        {
            std::cout << "\t" << extension.extensionName << "\n";
        }

        checkExtensionSupport(requiredExtensions.size(), extensionCount, requiredExtensions.data(), extensions);
    }

    void pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        for (const auto &device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE)
        {
            throw std::runtime_error("failed to find suitable GPU!");
        }
    }

    bool isDeviceSuitable(VkPhysicalDevice device)
    {
        // TODO
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        bool result = engine::findQueueFamilies(device).isComplete();
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

    void checkExtensionSupport(uint32_t glfwExtensionCount, uint32_t vkExtensionCount, const char **glfwExtensions, vector<VkExtensionProperties> &vkExtensions)
    {
        cout << "------------------\n";

        map<int, bool> supportedExtensions{};

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

            string supportString = "";

            if (supported)
                supportString = " is SUPPORTED";
            else
                supportString = " is NOT SUPPORTED";

            cout << vkExtensions[index].extensionName << supportString << "\n";
        }

        cout << "------------------\n";
    }

    bool checkValidationLayerSupport()
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

    void createLogicalDevice(){
        engine::QueueFamilyIndices indices = engine::findQueueFamilies(physicalDevice);

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = 0;
        if(enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS){
            throw std::runtime_error("failed to create logical device!");
        }
    }

    void acquireGraphicsQueue(){
        engine::QueueFamilyIndices indices = engine::findQueueFamilies(physicalDevice);
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
    }

    void initVulkan()
    {
        createInstance();
        pickPhysicalDevice();
        createLogicalDevice();
        acquireGraphicsQueue();
        createSurface();
    }

    void createSurface(){
        if (glfwCreateWindowSurface(instance, window.window, nullptr, &surface)!= VK_SUCCESS){
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window.window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyDevice(device, nullptr);
        vkDestroySurfaceKHR(instance, surface, nullptr);
        vkDestroyInstance(instance, nullptr);
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