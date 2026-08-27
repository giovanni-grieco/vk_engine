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
    {}


    void init()
    {
        EntityManager &em = EntityManager::getInstance();
        ComponentManager &cm = ComponentManager::getInstance();
        SystemManager &sm = SystemManager::getInstance();

        sm.registerSystem(std::make_unique<CameraSystem>(5.0f, 50.0f));
        //sm.registerSystem(std::make_unique<Translator>(1.0f));

        cm.registerComponent<TransformComponent>();
        cm.registerComponent<MeshComponent>();
        cm.registerComponent<CameraComponent>();
        cm.registerComponent<TextureComponent>();

        Entity camera = em.createEntity();
        cm.addComponent<CameraComponent>(camera, CameraComponent{});

        Entity floor = em.createEntity();
        MeshID floorMeshHandle = backend.addMesh(createMeshFromFile("models/quad.obj"));
        TextureID floorTextureHandle = backend.addTexture(createTextureFromFile("textures/floor.jpg"));

        TransformComponent floorTransform{};
        floorTransform.position.y = -1;
        floorTransform.rotation.z = 180;
        floorTransform.scale = glm::vec3(10.0f);
        cm.addComponent<TransformComponent>(floor, floorTransform);
        cm.addComponent<MeshComponent>(floor, MeshComponent{floorMeshHandle});
        cm.addComponent<TextureComponent>(floor, TextureComponent{floorTextureHandle});

        Mesh quadMesh {quadVertices, quadIndices};
        MeshID meshHandle = backend.addMesh(quadMesh);

        Texture tex = createTextureFromFile("textures/statue.jpg");
        TextureID texHandle = backend.addTexture(tex);

        Entity quad = em.createEntity();

        TransformComponent quadTransform {};
        quadTransform.position.z = -1;

        cm.addComponent<TransformComponent>(quad, quadTransform);
        cm.addComponent<MeshComponent>(quad, MeshComponent{meshHandle});
        cm.addComponent<TextureComponent>(quad, TextureComponent{texHandle});

        
        Entity house = em.createEntity();

        Mesh houseMesh = createMeshFromFile("models/viking_room.obj");
        MeshID houseMeshHandle = backend.addMesh(houseMesh);

        Texture houseTexture = createTextureFromFile("textures/viking_room.png");
        TextureID houseTextureHandle = backend.addTexture(houseTexture);

        TransformComponent houseTransform {};
        houseTransform.position.y = -1;
        houseTransform.rotation.x = -90;
        houseTransform.rotation.y = -90;


        cm.addComponent<TransformComponent>(house, houseTransform);
        cm.addComponent<MeshComponent>(house, MeshComponent{houseMeshHandle});
        cm.addComponent<TextureComponent>(house, TextureComponent{houseTextureHandle});
        


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