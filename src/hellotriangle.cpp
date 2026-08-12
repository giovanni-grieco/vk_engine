#include "platform/window.hpp"
#include "vulkan/vk_instance.hpp"
#include "vulkan/vk_device.hpp"
#include "vulkan/vk_surface.hpp"
#include "vulkan/vk_queues.hpp"
#include "vulkan/vk_swap_chain.hpp"
#include "vulkan/vk_render_pass.hpp"
#include "vulkan/vk_pipeline_layout.hpp"
#include "vulkan/vk_pipeline.hpp"
#include "vulkan/vk_shader.hpp"
#include "vulkan/vk_framebuffers.hpp"

class HelloTriangleApplication
{
public:
    HelloTriangleApplication() : window(WIDTH, HEIGHT, applicationName),
                                 instance(applicationName, window, enableValidationLayers, validationLayers),
                                 surface(instance, window),
                                 device(instance, surface, deviceExtensions),
                                 queues(device, surface),
                                 swapChain(device, surface, window),
                                 renderPass(device, swapChain),
                                 pipelineLayout(device),
                                 pipeline(device, swapChain, pipelineLayout, renderPass, shadersFilePaths, shaderTypes),
                                 frameBuffers(device, swapChain, renderPass) 
                                 {}

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
    engine::VulkanInstance instance;
    engine::VulkanSurface surface;
    engine::VulkanDevice device;
    engine::VulkanQueues queues;
    engine::VulkanSwapChain swapChain;
    engine::VulkanRenderPass renderPass;
    engine::VulkanPipelineLayout pipelineLayout;
    engine::VulkanPipeline pipeline;
    engine::VulkanFramebuffers frameBuffers;

    void mainLoop()
    {
        //throw std::runtime_error("Failed to run main loop");
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