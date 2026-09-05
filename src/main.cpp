#include "ecs/component_manager.hpp"
#include "ecs/scene_manager.hpp"
#include "ecs/system_manager.hpp"

#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/parent.hpp"
#include "ecs/components/children.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/texture.hpp"
#include "ecs/components/point_light.hpp"
#include "ecs/components/ambient_light.hpp"
#include "ecs/components/directional_light.hpp"
#include "ecs/components/player_ship.hpp"
#include "ecs/components/laser_bolt.hpp"

#include "ecs/systems/transform_system.hpp"
#include "ecs/systems/free_camera_system.hpp"
#include "ecs/systems/player_ship_system.hpp"
#include "ecs/systems/point_light_system.hpp"
#include "ecs/systems/laser_system.hpp"

#include "time/engine_time.hpp"

#include "input/input_system.hpp"
#include "rendering/renderer.hpp"
#include "platform/window.hpp"
#include "vulkan/vk_backend.hpp"

#include "geometry/vertex.hpp"
#include "geometry/mesh.hpp"

#include "texture/texture.hpp"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace engine;

class Game
{
public:
    Window &window;
    VulkanBackend &backend;
    Renderer &renderer;

    Game(Window &window, VulkanBackend &backend, Renderer &renderer)
        : window(window),
          backend(backend),
          renderer(renderer)
    {}


    void init() const
    {
        SceneManager &sm = SceneManager::getInstance();
        ComponentManager &cm = ComponentManager::getInstance();
        SystemManager &sysmg = SystemManager::getInstance();
        
        cm.registerComponent<LocalTransformComponent>();
        cm.registerComponent<WorldTransformComponent>();
        cm.registerComponent<ParentComponent>();
        cm.registerComponent<ChildrenComponent>();
        cm.registerComponent<PointLightComponent>();
        cm.registerComponent<AmbientLightComponent>();
        cm.registerComponent<DirectionalLightComponent>();
        cm.registerComponent<MeshComponent>();
        cm.registerComponent<TextureComponent>();
        cm.registerComponent<CameraComponent>();

        /*Entity camera = sm.createEntity();
        LocalTransformComponent cameraLocal {};
        cameraLocal.position = glm::vec3(0.0f, 0.25f, 0.0f);
        cm.addComponent<LocalTransformComponent>(camera, cameraLocal);
        cm.addComponent<WorldTransformComponent>(camera, WorldTransformComponent{});
        cm.addComponent<CameraComponent>(camera, CameraComponent{});

        sysmg.registerSystem(std::make_unique<FreeCameraSystem>());*/

        Entity floor = sm.createEntity();
        LocalTransformComponent floorLocal {};
        floorLocal.rotation *= glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        floorLocal.scale = glm::vec3(100.0f);
        cm.addComponent<LocalTransformComponent>(floor, floorLocal);
        cm.addComponent<WorldTransformComponent>(floor, WorldTransformComponent{});
        cm.addComponent<MeshComponent>(floor, MeshComponent{backend.addMesh(createMeshFromFile("../models/quad.obj"))});
        cm.addComponent<TextureComponent>(floor, TextureComponent{{backend.addTexture(createTextureFromFile("../textures/floor.jpg"))}});


        Entity ambientlight = sm.createEntity();
        cm.addComponent<AmbientLightComponent>(ambientlight, AmbientLightComponent{});

        cm.registerComponent<PlayerShip>();
        
        Entity tieFighter = sm.createEntity();
        Mesh tieMesh = createMeshFromFile("../models/tie.obj");
        MeshID tieMeshHandle = backend.addMesh(tieMesh);
        LocalTransformComponent tieTransform {};
        tieTransform.position.y = 1;
        tieTransform.position.z = -2;
        tieTransform.scale = glm::vec3{0.25f};
        cm.addComponent<WorldTransformComponent>(tieFighter, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(tieFighter, tieTransform);
        cm.addComponent<MeshComponent>(tieFighter, MeshComponent{tieMeshHandle});
        cm.addComponent<PlayerShip>(tieFighter, PlayerShip{1.0f});

        float followDistance = 6.0f;
        float followHeight = 1.0f;
        Entity camera = sm.createEntity();
        LocalTransformComponent cameraTransform{};
        cameraTransform.position = glm::vec3(0.0f, followHeight, -followDistance) / tieTransform.scale;
        cm.addComponent<WorldTransformComponent>(camera, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(camera, cameraTransform);
        cm.addComponent<CameraComponent>(camera, CameraComponent{});
        sm.parent(camera, tieFighter); // Camera is child of tieFighter!!

        cm.registerComponent<LaserBoltComponent>();
        Mesh laserMesh = createMeshFromFile("../models/laserbolt.obj");
        MeshID laserMeshHandle = backend.addMesh(laserMesh);
        Texture laserTexture = createBrightGreenLaserTexture(64, 64);
        TextureID laserTexHandle = backend.addTexture(laserTexture);

        float minSpeed = 0.0f, maxSpeed = 10.0f, angularSpeed = 55.0f;
        sysmg.registerSystem(std::make_unique<PlayerShipSystem>(minSpeed, maxSpeed, angularSpeed, PlayerShipSystem::Ctx{laserMeshHandle, laserTexHandle}));
        sysmg.registerSystem(std::make_unique<LaserSystem>());

        Entity directionalLight = sm.createEntity();
        DirectionalLightComponent dlc {};
        dlc.direction = {0.0f, -1.0f, 0.0f};
        dlc.color = {1.0f, 1.0f, 1.0f};
        dlc.intensity = 0.15f;
        cm.addComponent<DirectionalLightComponent>(directionalLight, dlc);

        sysmg.start();
    }

    void run() const
    {

        EngineTime &time = EngineTime::getInstance();
        SystemManager &sysmg = SystemManager::getInstance();
        while (!window.shouldWindowClose())
        {
            time.beginFrame();
            window.pollEvents();
            sysmg.update();
            renderer.render();
            Input::getInstance().endFrame();
            window.setWindowTitle("VK Engine - FPS: " + std::to_string(time.fps()));
        }
        backend.waitForIdle();
    }
};

int main()
{
    constexpr int width = 1920;
    constexpr int height = 1080;
    constexpr bool validationLayer = true;
    const std::string applicationName = "VK Engine";
    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

    const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    const std::vector<std::string> shadersFilePaths = {"../shaders/shader.vert.spv", "../shaders/shader.frag.spv"};
    const std::vector<engine::VulkanShaderType> shaderTypes = {engine::VulkanShaderType::VERTEX, engine::VulkanShaderType::FRAGMENT};

    std::cout << "vk_engine started!\n"
              << "-----------------\n";

    Window window{width, height, applicationName};

    VulkanBackend backend{applicationName, validationLayer, validationLayers, deviceExtensions, shadersFilePaths, shaderTypes, window};

    Renderer renderer{window, backend};

    const Game game{window, backend, renderer};

    game.init();
    game.run();

    std::cout << "-----------------\n"
              << "vk_engine closing!\n";
}