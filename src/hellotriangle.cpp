#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"
#include "rendering/frame_info.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "vulkan/vk_swap_chain.hpp"

#include <chrono>

using namespace std::chrono;

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

    std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
    double avgFps = 0.0;


    engine::FrameInfo makeFrameInfo(engine::VulkanSwapChain& swapChain){
        engine::FrameInfo frameInfo{};
        frameInfo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        frameInfo.projection = glm::perspective(glm::radians(45.0f), swapChain.extent.width / (float) swapChain.extent.height, 0.1f, 10.0f);
        frameInfo.projection[1][1] *= -1;
        return frameInfo;
    }

    void mainLoop()
    {
        // throw std::runtime_error("Failed to run main loop");
        while (!window.shouldWindowClose())
        {
            glfwPollEvents();
            auto now = std::chrono::steady_clock::now();
            double dt = std::chrono::duration<double>(now - last).count();
            last = now;

            // exponential moving average
            double fps = 1.0 / dt;
            avgFps = avgFps == 0.0 ? fps : avgFps * 0.95 + fps * 0.05;


            engine::FrameInfo frameInfo = makeFrameInfo(vulkanBackend.swapChain);
            vulkanBackend.drawFrame(window, frameInfo);

            std::string title = applicationName + " - FPS: "+ std::to_string(avgFps);
            window.setWindowTitle(title);
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