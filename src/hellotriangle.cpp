#include "platform/window.hpp"
#include "vulkan/vk_instance.hpp"
#include "vulkan/vk_device.hpp"
#include "vulkan/vk_surface.hpp"
#include "vulkan/vk_queues.hpp"

class HelloTriangleApplication
{
public:
    HelloTriangleApplication() : 
    window(WIDTH, HEIGHT, applicationName) , 
    instance(applicationName, window, enableValidationLayers, validationLayers), 
    surface(instance, window),
    device(instance, surface, enableValidationLayers, validationLayers),
    queues(device, surface) {}

    void run()
    {
        mainLoop();
    }


private:
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;

    const std::string applicationName = "Hello Triangle";

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