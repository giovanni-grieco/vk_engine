#include "camera_system.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/player_ship.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity/entity.hpp"
#include <glm/glm.hpp>
#include <stdexcept>

namespace engine
{
    CameraSystem::CameraSystem(float followDistance, float followHeight){
        this->followDistance = followDistance;
        this->followHeight = followHeight;
    }

    void CameraSystem::start()
    {
    }

    void CameraSystem::update()
    {
        auto &cm = ComponentManager::getInstance();

        const auto &cameraEntities = cm.getEntitiesWithComponent<CameraComponent>();
        if (cameraEntities.empty())
        {
            throw std::runtime_error("Camera System did not find any Entity with CameraComponent!");
        }
        Entity camera = cameraEntities[0];

        const auto &shipEntities = cm.getEntitiesWithComponent<PlayerShip>();
        if (shipEntities.empty())
        {
            throw std::runtime_error("Camera System did not find any Entity with PlayerShip component to follow!");
        }
        Entity playerShip = shipEntities[0];

        const WorldTransformComponent &world = cm.getComponent<WorldTransformComponent>(playerShip);
        const LocalTransformComponent &shipLocal = cm.getComponent<LocalTransformComponent>(playerShip);

        glm::vec3 shipPos = glm::vec3(world.matrix[3]);
        glm::vec3 shipForward = glm::normalize(glm::vec3(world.matrix[2]));
        glm::vec3 shipUp = glm::normalize(glm::vec3(world.matrix[1])); // model's +Y

        glm::vec3 targetPos = shipPos - shipForward * followDistance + shipUp * followHeight;

        LocalTransformComponent &cameraLocal = cm.getComponent<LocalTransformComponent>(camera);
        cameraLocal.position = targetPos;
        cameraLocal.rotation = shipLocal.rotation;
        cameraLocal.scale = glm::vec3(1.0f);
    }
}