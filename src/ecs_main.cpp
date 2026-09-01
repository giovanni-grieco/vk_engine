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

#include "ecs/systems/transform_system.hpp"
#include "ecs/systems/camera_system.hpp"
#include "ecs/systems/point_light_system.hpp"

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

        sysmg.registerSystem(std::make_unique<TransformSystem>());
        sysmg.registerSystem(std::make_unique<CameraSystem>(5.0f, 50.0f));
        sysmg.registerSystem(std::make_unique<PointLightSystem>());
    
        //sm.registerSystem(std::make_unique<Translator>(1.0f));

        cm.registerComponent<LocalTransformComponent>();
        cm.registerComponent<WorldTransformComponent>();
        cm.registerComponent<ParentComponent>();
        cm.registerComponent<ChildrenComponent>();
        cm.registerComponent<PointLightComponent>();
        cm.registerComponent<AmbientLightComponent>();
        cm.registerComponent<DirectionalLightComponent>();
        cm.registerComponent<MeshComponent>();
        cm.registerComponent<CameraComponent>();
        cm.registerComponent<TextureComponent>();

        Entity camera = sm.createEntity();
        cm.addComponent<CameraComponent>(camera, CameraComponent{});

        Entity ambientLight = sm.createEntity();
        AmbientLightComponent alc {};
        alc.color = {1.0f, 1.0f, 1.0f};
        alc.intensity = 0.01f;
        cm.addComponent<AmbientLightComponent>(ambientLight, alc);

        Entity directionalLight = sm.createEntity();
        DirectionalLightComponent dlc {};
        dlc.direction = {0.0f, -1.0f, 0.0f};
        dlc.color = {1.0f, 1.0f, 1.0f};
        dlc.intensity = 0.1f;
        cm.addComponent<DirectionalLightComponent>(directionalLight, dlc);

        Entity pointLight = sm.createEntity();
        LocalTransformComponent lightTransform{};
        lightTransform.position = glm::vec3(2.0f, 3.0f, 2.0f);
        cm.addComponent<WorldTransformComponent>(pointLight, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(pointLight, lightTransform);
        PointLightComponent plc {};
        plc.color = glm::vec3(1.0f, 0.5f, 0.5f);
        plc.intensity = 0.5f;
        cm.addComponent<PointLightComponent>(pointLight, plc);

        Entity floor = sm.createEntity();
        MeshID floorMeshHandle = backend.addMesh(createMeshFromFile("../models/quad.obj"));
        TextureID floorTextureHandle = backend.addTexture(createTextureFromFile("../textures/floor.jpg"));

        LocalTransformComponent floorTransform{};
        floorTransform.position.y = -1;
        floorTransform.rotation.z = 180;
        floorTransform.scale = glm::vec3(10.0f);
        cm.addComponent<WorldTransformComponent>(floor, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(floor, floorTransform);
        cm.addComponent<MeshComponent>(floor, MeshComponent{floorMeshHandle});
        cm.addComponent<TextureComponent>(floor, TextureComponent{floorTextureHandle});

        Mesh quadMesh {.vertices = quadVertices, .indices = quadIndices};
        MeshID meshHandle = backend.addMesh(quadMesh);

        Texture tex = createTextureFromFile("../textures/statue.jpg");
        TextureID texHandle = backend.addTexture(tex);

        Entity quad = sm.createEntity();

        LocalTransformComponent quadTransform {};
        quadTransform.position.z = -2;

        cm.addComponent<WorldTransformComponent>(quad, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(quad, quadTransform);
        cm.addComponent<MeshComponent>(quad, MeshComponent{meshHandle});
        cm.addComponent<TextureComponent>(quad, TextureComponent{texHandle});


        Entity house = sm.createEntity();

        Mesh houseMesh = createMeshFromFile("../models/viking_room.obj");
        MeshID houseMeshHandle = backend.addMesh(houseMesh);

        Texture houseTexture = createTextureFromFile("../textures/viking_room.png");
        TextureID houseTextureHandle = backend.addTexture(houseTexture);

        LocalTransformComponent houseTransform {};
        houseTransform.position.y = -0.9f;
        houseTransform.rotation.x = -90;
        houseTransform.rotation.y = -90;


        cm.addComponent<WorldTransformComponent>(house, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(house, houseTransform);
        cm.addComponent<MeshComponent>(house, MeshComponent{houseMeshHandle});
        cm.addComponent<TextureComponent>(house, TextureComponent{houseTextureHandle});

        Entity tieFighter = sm.createEntity();

        Mesh tieMesh = createMeshFromFile("../models/tie.obj");
        tieMesh.dump();
        MeshID tieMeshHandle = backend.addMesh(tieMesh);
        LocalTransformComponent tieTransform {};
        tieTransform.position.y = 10;
        tieTransform.position.z = -2;

        cm.addComponent<WorldTransformComponent>(tieFighter, WorldTransformComponent{});
        cm.addComponent<LocalTransformComponent>(tieFighter, tieTransform);
        cm.addComponent<MeshComponent>(tieFighter, MeshComponent{tieMeshHandle});



        sysmg.start();
    }

    void run() const
    {

        EngineTime &time = EngineTime::getInstance();
        SystemManager &sysmg = SystemManager::getInstance();
        while (!window.shouldWindowClose())
        {
            window.pollEvents();
            time.beginFrame();
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