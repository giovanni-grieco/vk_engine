#include "camera_system.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/player_ship.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/entity/entity.hpp"
#include <glm/glm.hpp>

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
        if (cm.getEntitiesWithComponent<CameraComponent>().size() == 0)
        {
            throw std::runtime_error("Camera System did not find any Entity with CameraComponent!");
        }

        CameraComponent &camera = cm.getComponent<CameraComponent>(cm.getEntitiesWithComponent<CameraComponent>()[0]);

        if (cm.getEntitiesWithComponent<PlayerShip>().size() > 0)
        {
            Entity playerShip = cm.getEntitiesWithComponent<PlayerShip>()[0];
            const WorldTransformComponent &world = cm.getComponent<WorldTransformComponent>(playerShip);

            glm::vec3 shipPos = glm::vec3(world.matrix[3]);
            glm::vec3 shipForward = glm::normalize(glm::vec3(world.matrix[2]));
            glm::vec3 shipUp = glm::normalize(glm::vec3(world.matrix[1])); // model's +Y

            glm::vec3 targetPos = shipPos - shipForward * followDistance + shipUp * followHeight;

            camera.forward = shipForward;
            camera.up = shipUp;
            camera.position = targetPos;
        }
        else
        {
            throw std::runtime_error("Camera System did not find any Entity with PlayerShip component to follow!");
        }
    }
}