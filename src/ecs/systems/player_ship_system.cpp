#include "player_ship_system.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"
#include "ecs/components/player_ship.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/point_light.hpp"
#include "ecs/components/laser_bolt.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/texture.hpp"
#include "ecs/entity/entity.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/scene_manager.hpp"

#include "utils/angles.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <iostream>

namespace engine
{
    PlayerShipSystem::PlayerShipSystem(float minSpeed, float maxSpeed, float angularSpeed, Ctx context)
    {
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        this->angularSpeed = angularSpeed;
        this->currentSpeed = minSpeed;
        this->laserBoltMeshHandle = context.laserBoltMeshHandle;
        this->laserBoltTexHandle = context.laserBoltTexHandle;
    }

    void PlayerShipSystem::start()
    {
    }

    // Clamps a throttle change, mirroring how the free camera clamps its speed.
    float updateSpeed(float currentSpeed, float delta, float minSpeed, float maxSpeed)
    {
        float candidateSpeed = currentSpeed + delta;

        if (delta > 0)
            candidateSpeed = std::min(candidateSpeed, maxSpeed);
        else if (delta < 0)
            candidateSpeed = std::max(candidateSpeed, minSpeed);

        return candidateSpeed;
    }

    void PlayerShipSystem::shootLaser(Entity ship)
    {
        auto &sm = SceneManager::getInstance();
        auto &cm = ComponentManager::getInstance();
        auto &time = EngineTime::getInstance();

        if (time.totalTime() - this->lastTimeFired >= timeBetweenFires)
        {
            this->lastTimeFired = time.totalTime();
            for (int i = 0; i < 2; i++)
            {
                Entity laser = sm.createEntity();
                LocalTransformComponent local{};
                WorldTransformComponent world{};
                const LocalTransformComponent& shipLocal = cm.getComponent<LocalTransformComponent>(ship);
                const WorldTransformComponent& shipWorld = cm.getComponent<WorldTransformComponent>(ship);
                const glm::vec3 forward = glm::normalize(glm::vec3(shipWorld.matrix[2]));
                const glm::vec3 up = glm::normalize(glm::vec3(shipWorld.matrix[1]));
                const glm::vec3 right = glm::normalize(glm::cross(forward, up));

                float forwardCoefficient = 0.3f;
                float lateralDistanceCoeff = 0.1f;
                float verticalDistanceCoeff = -0.1f;


                local = shipLocal;
                world = shipWorld;
                local.position += forward * forwardCoefficient;
                if(i==0){
                    local.position += right * lateralDistanceCoeff;
                }else{
                    local.position -= right * lateralDistanceCoeff;
                }

                local.position += up * verticalDistanceCoeff;

                local.scale = glm::vec3(1.0f);

                cm.addComponent<LocalTransformComponent>(laser, local);
                cm.addComponent<WorldTransformComponent>(laser, world);
                cm.addComponent<PointLightComponent>(laser, PointLightComponent{{0.0f, 2.0f, 0.5f}, 1.0f});
                cm.addComponent<LaserBoltComponent>(laser, LaserBoltComponent{});
                cm.addComponent<MeshComponent>(laser, MeshComponent{this->laserBoltMeshHandle});
                cm.addComponent<TextureComponent>(laser, TextureComponent{{this->laserBoltTexHandle}});
            }
        }
    }

    void PlayerShipSystem::handleInputs(Entity ship)
    {
        ComponentManager &cm = ComponentManager::getInstance();
        Input &input = Input::getInstance();

        float dt = EngineTime::getInstance().deltaTime();

        LocalTransformComponent &local = cm.getComponent<LocalTransformComponent>(ship);
        const WorldTransformComponent &world = cm.getComponent<WorldTransformComponent>(ship);

        const glm::vec3 forward = glm::normalize(glm::vec3(world.matrix[2]));
        const glm::vec3 up = glm::normalize(glm::vec3(world.matrix[1]));
        const glm::vec3 right = glm::normalize(glm::cross(forward, up));


        if (input.isKeyDown(Key::W))
            currentSpeed = updateSpeed(currentSpeed, THROTTLE_RATE * dt, minSpeed, maxSpeed);
        if (input.isKeyDown(Key::S))
            currentSpeed = updateSpeed(currentSpeed, -THROTTLE_RATE * dt, minSpeed, maxSpeed);

        local.position += forward * currentSpeed * dt;

        const float angle = glm::radians(angularSpeed * dt);


        if (input.isKeyDown(Key::Left))
            local.rotation = local.rotation * glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        if (input.isKeyDown(Key::Right))
            local.rotation = local.rotation * glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));


        if (input.isKeyDown(Key::Down))
            local.rotation = local.rotation * glm::angleAxis(-angle, glm::vec3(1.0f, 0.0f, 0.0f));
        if (input.isKeyDown(Key::Up))
            local.rotation = local.rotation * glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));

        if (input.isKeyDown(Key::A))
            local.rotation = local.rotation * glm::angleAxis(-angle, glm::vec3(0.0f, 0.0f, 1.0f));
        if (input.isKeyDown(Key::D))
            local.rotation = local.rotation * glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));

        local.rotation = glm::normalize(local.rotation);

        if (input.isKeyDown(Key::Space))
            shootLaser(ship);
    }

    void PlayerShipSystem::update()
    {
        ComponentManager &cm = ComponentManager::getInstance();

        auto ships = cm.getEntitiesWithComponent<PlayerShip>();
        if (ships.size() > 1)
        {
            std::cout << "More than one player ship! returning...\n";
            return;
        }
        else if (ships.size() < 1)
        {
            std::cout << "No player ship exists! returning...\n";
            return;
        }

        handleInputs(ships[0]);
    }
}