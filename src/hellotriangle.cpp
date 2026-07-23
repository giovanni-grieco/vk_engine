#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <map>

using namespace std;

class HelloTriangleApplication
{
public:
    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    string applicationName = "Hello Triangle";

    const std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation"};

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    GLFWwindow *window;
    VkInstance instance;

    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, applicationName.c_str(), nullptr, nullptr);
    }

    void initVulkan()
    {
        createInstance();
    }

    void createInstance()
    {
        cout <<"Enable validation layers: \t " <<enableValidationLayers << "\n";
        cout <<"Check validation layer support:  "<< checkValidationLayerSupport() << "\n";
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

        if(enableValidationLayers){
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        }else{
            createInfo.enabledLayerCount = 0;
        }


        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (int i = 0; i < glfwExtensionCount; i++)
        {
            cout << "\t" << glfwExtensions[i] << "\n";
        }

        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

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

        checkExtensionSupport(glfwExtensionCount, extensionCount, glfwExtensions, extensions);
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
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

    
};

/*int main()
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
}*/