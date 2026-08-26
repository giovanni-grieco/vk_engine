#include "ecs/component_manager.hpp"
#include "ecs/entity_manager.hpp"
#include "ecs/system_manager.hpp"

#include "ecs/components/transform.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/texture.hpp"

#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/translator_system.hpp"

#include "time/engine_time.hpp"

#include "input/input_system.hpp"
#include "rendering/renderer.hpp"
#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"

#include "geometry/vertex.hpp"
#include "geometry/mesh.hpp"

#include "texture/texture.hpp"

#include <memory>

using namespace engine;

class Game
{
public:
    Window &window;
    VulkanBackend &backend;
    Renderer &renderer;

    Game(Window &window, Renderer &renderer, VulkanBackend &backend)
        : window(window),
          renderer(renderer),
          backend(backend)
    {
    }
    void init()
    {
        EntityManager &em = EntityManager::getInstance();
        ComponentManager &cm = ComponentManager::getInstance();
        SystemManager &sm = SystemManager::getInstance();

        sm.registerSystem(std::make_unique<CameraSystem>(5.0f, 50.0f));
        sm.registerSystem(std::make_unique<Translator>(1.0f));

        cm.registerComponent<TransformComponent>();
        cm.registerComponent<MeshComponent>();
        cm.registerComponent<CameraComponent>();
        cm.registerComponent<TextureComponent>();

        Entity camera = em.createEntity();
        cm.addComponent<CameraComponent>(camera, CameraComponent{});

        Mesh quad {quadVertices, quadIndices};
        MeshID meshHandle = backend.addMesh(quad);

        Texture tex = createTextureFromFile("textures/texture.jpg");
        TextureID texHandle = backend.addTexture(tex);

        Entity gameObject = em.createEntity();
        cm.addComponent<MeshComponent>(gameObject, MeshComponent{meshHandle});
        cm.addComponent<TransformComponent>(gameObject, TransformComponent{});
        cm.addComponent<TextureComponent>(gameObject, TextureComponent{texHandle});

        sm.start();
    }

    void run()
    {

        EngineTime &time = EngineTime::getInstance();
        SystemManager &sm = SystemManager::getInstance();
        while (!window.shouldWindowClose())
        {
            window.pollEvents();
            time.beginFrame();
            sm.update();
            renderer.render();
            Input::getInstance().endFrame();
            window.setWindowTitle("VK Engine - FPS: " + std::to_string(time.fps()));
        }
        backend.waitForIdle();
    }
};

int main()
{
    const int width = 1920;
    const int height = 1080;
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

    Game game{window, renderer, backend};

    game.init();
    game.run();

    std::cout << "-----------------\n"
              << "vk_engine closing!\n";
}