#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"

#include "ecs/systems/camera_system.hpp"

#include "time/engine_time.hpp"

#include "rendering/renderer.hpp"
#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"

#include <memory>

using namespace engine;

void initComponents(){
    ComponentManager &cm = ComponentManager::getInstance();

    cm.registerComponent<TransformComponent>();
    cm.registerComponent<MeshComponent>();
    cm.registerComponent<CameraComponent>();
}

void initSystems(){
    SystemManager &sm = SystemManager::getInstance();

    sm.registerSystem(std::make_unique<CameraSystem>(0.3f));

    sm.start();
}

int main()
{
    const int width = 800;
    const int height = 600;
    const bool validationLayer = true;
    const std::string applicationName = "VK Engine";
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<std::string> shadersFilePaths = {"shaders/triangle.vert.spv", "shaders/triangle.frag.spv"};
    const std::vector<engine::VulkanShaderType> shaderTypes = {engine::VulkanShaderType::VERTEX, engine::VulkanShaderType::FRAGMENT};


    std::cout << "vk_engine started!\n"
              << "-----------------\n";

    Window window{width, height, applicationName};

    VulkanBackend backend{applicationName, validationLayer, validationLayers, deviceExtensions, shadersFilePaths, shaderTypes, window};

    Renderer renderer{window, backend};

    EntityManager &em = EntityManager::getInstance();
    ComponentManager &cm = ComponentManager::getInstance();
    SystemManager &sm = SystemManager::getInstance();

    initComponents();
    initSystems();

    while (!window.shouldWindowClose())
    {   
        window.pollEvents();
        EngineTime::getInstance().beginFrame();
        sm.update();
        renderer.render();
    }
    backend.waitForIdle();

    std::cout << "-----------------\n"
              << "vk_engine closing!\n";
}