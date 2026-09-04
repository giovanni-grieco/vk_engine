#include "player_ship_system.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"
#include "ecs/components/player_ship.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/point_light.hpp"
#include "ecs/components/laser_bolt.hpp"
#include "ecs/entity/entity.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/scene_manager.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <iostream>

namespace engine
{
    PlayerShipSystem::PlayerShipSystem(float minSpeed, float maxSpeed, float angularSpeed)
    {
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        this->angularSpeed = angularSpeed;
        this->currentSpeed = minSpeed;
    }

    void PlayerShipSystem::start() {}

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

    void PlayerShipSystem::shootLaser(Entity ship){
        auto& sm = SceneManager::getInstance();
        auto& cm = ComponentManager::getInstance();
        auto& time = EngineTime::getInstance();

        if(time.totalTime() - this->lastTimeFired >= timeBetweenFires){
            this->lastTimeFired = time.totalTime();
            Entity laser = sm.createEntity();
            LocalTransformComponent local{};
            WorldTransformComponent world{};
            cm.addComponent<LocalTransformComponent>(laser, local);
            cm.addComponent<WorldTransformComponent>(laser, world);
            cm.addComponent<PointLightComponent>(laser, PointLightComponent{{0.0f, 2.0f, 0.5f}, 1.0f});
            cm.addComponent<LaserBoltComponent>(laser, LaserBoltComponent{});

        }
    }

    void PlayerShipSystem::handleInputs(Entity ship)
    {
        ComponentManager &cm = ComponentManager::getInstance();
        Input &input = Input::getInstance();

        float dt = EngineTime::getInstance().deltaTime();

        LocalTransformComponent &local = cm.getComponent<LocalTransformComponent>(ship);
        const WorldTransformComponent &world = cm.getComponent<WorldTransformComponent>(ship);

        // Basis from the PREVIOUS frame's world matrix. The transform system runs
        // before us, so edits to the local transform show up one frame later.
        const glm::vec3 forward = glm::normalize(glm::vec3(world.matrix[2]));

        // Throttle: W accelerates, S decelerates (clamped to [min, max]).
        if (input.isKeyDown(Key::W))
            currentSpeed = updateSpeed(currentSpeed, THROTTLE_RATE * dt, minSpeed, maxSpeed);
        if (input.isKeyDown(Key::S))
            currentSpeed = updateSpeed(currentSpeed, -THROTTLE_RATE * dt, minSpeed, maxSpeed);

        // Always fly along the nose.
        local.position += forward * currentSpeed * dt;

        // Rotation this frame, in degrees (LocalTransformComponent stores Euler degrees).
        const float angle = angularSpeed * dt;

        // Yaw around the world up axis. The nose is local +Z, so a positive
        // Y-rotation steers toward +X (the ship's left) — hence the flipped signs.
        if (input.isKeyDown(Key::Left))
            local.rotation.y += angle;
        if (input.isKeyDown(Key::Right))
            local.rotation.y -= angle;

        // Pitch around the local X axis. Positive X-rotation drops the nose.
        if (input.isKeyDown(Key::Up))
            local.rotation.x -= angle;
        if (input.isKeyDown(Key::Down))
            local.rotation.x += angle;

        // Roll around the nose (local +Z). D banks right, A banks left.
        if (input.isKeyDown(Key::A))
            local.rotation.z -= angle;
        if (input.isKeyDown(Key::D))
            local.rotation.z += angle;

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