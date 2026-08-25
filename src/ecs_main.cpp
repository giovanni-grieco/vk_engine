#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"

#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/translator_system.hpp"

#include "time/engine_time.hpp"

#include "input/input_system.hpp"
#include "rendering/renderer.hpp"
#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"

#include "texture/texture.hpp"

#include <memory>

using namespace engine;

void init(){
    EntityManager &em = EntityManager::getInstance();
    ComponentManager &cm = ComponentManager::getInstance();
    SystemManager &sm = SystemManager::getInstance();

    sm.registerSystem(std::make_unique<CameraSystem>(5.0f, 50.0f));
    //sm.registerSystem(std::make_unique<Translator>(0.3f));



    cm.registerComponent<TransformComponent>();
    cm.registerComponent<MeshComponent>();
    cm.registerComponent<CameraComponent>();
    Entity camera = em.createEntity();

    cm.addComponent<CameraComponent>(camera, CameraComponent{});



    sm.start();
}

void runMainLoop(Window& window, SystemManager& sm, Renderer& renderer, VulkanBackend& backend){
    init();

    EngineTime& time = EngineTime::getInstance();

    while (!window.shouldWindowClose())
    {   
        window.pollEvents();
        time.beginFrame();
        sm.update();
        renderer.render();
        Input::getInstance().endFrame();
        window.setWindowTitle("VK Engine - FPS: "+std::to_string(time.fps()));
    }
    backend.waitForIdle();
}

int main()
{
    const int width = 800;
    const int height = 600;
    const bool validationLayer = true;
    const std::string applicationName = "VK Engine";
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<std::string> shadersFilePaths = {"shaders/shader.vert.spv", "shaders/shader.frag.spv"};
    const std::vector<engine::VulkanShaderType> shaderTypes = {engine::VulkanShaderType::VERTEX, engine::VulkanShaderType::FRAGMENT};


    std::cout << "vk_engine started!\n"
              << "-----------------\n";

    Window window{width, height, applicationName};

    VulkanBackend backend{applicationName, validationLayer, validationLayers, deviceExtensions, shadersFilePaths, shaderTypes, window};

    Renderer renderer{window, backend};

    EntityManager &em = EntityManager::getInstance();
    ComponentManager &cm = ComponentManager::getInstance();
    SystemManager &sm = SystemManager::getInstance();
    Texture tex{};
    //Texture tex = createTextureFromFile("textures/texture.jpg");
    createTextureFromFile(tex, "textures/texture.jpg");
    tex.dump();

    runMainLoop(window, sm, renderer, backend);

    std::cout << "-----------------\n"
              << "vk_engine closing!\n";
}