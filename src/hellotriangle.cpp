#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"

class HelloTriangleApplication
{
public:
    HelloTriangleApplication() : window(WIDTH, HEIGHT, applicationName),
                                 vulkanBackend(applicationName, enableValidationLayers, validationLayers, deviceExtensions, shadersFilePaths, shaderTypes, window)
    {
    }

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


    std::vector<std::string> shadersFilePaths = {"shaders/triangle.vert.spv", "shaders/triangle.frag.spv"};
    std::vector<engine::VulkanShaderType> shaderTypes = {engine::VulkanShaderType::VERTEX, engine::VulkanShaderType::FRAGMENT};

    engine::Window window;
    engine::VulkanBackend vulkanBackend;


    void mainLoop()
    {
        // throw std::runtime_error("Failed to run main loop");
        while (!glfwWindowShouldClose(window.window))
        {
            glfwPollEvents();
            vulkanBackend.drawFrame(window);
        }

        vulkanBackend.waitForIdle();
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