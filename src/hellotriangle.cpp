#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <set>

#include "platform/window.hpp"
#include "vulkan/vk_instance.hpp"
#include "vulkan/vk_device.hpp"
#include "vulkan/vk_surface.hpp"
#include "vulkan/vk_queues.hpp"

using namespace std;

class HelloTriangleApplication
{
public:
    HelloTriangleApplication() : 
    window(WIDTH, HEIGHT, applicationName) , 
    instance(applicationName, window, enableValidationLayers, validationLayers), 
    surface(instance.instance, window),
    device(instance.instance, surface.surface, enableValidationLayers, validationLayers),
    queues(device, surface) {}

    void run()
    {
        mainLoop();
    }


private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    string applicationName = "Hello Triangle";

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    engine::Window window;
    engine::VulkanInstance instance;
    engine::VulkanSurface surface;
    engine::VulkanDevice device;
    engine::VulkanQueues queues;


    void mainLoop()
    {
        while (!glfwWindowShouldClose(window.window))
        {
            glfwPollEvents();
        }
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