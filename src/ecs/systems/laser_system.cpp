#include "laser_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/laser_bolt.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "time/engine_time.hpp"
#include "ecs/scene_manager.hpp"

namespace engine
{
    void LaserSystem::start()
    {
    }

    void LaserSystem::update()
    {
        auto& cm = ComponentManager::getInstance();
        auto& time = EngineTime::getInstance();
        auto& sm = SceneManager::getInstance();

        // Track newly spawned laser bolts: record their "birth" time.
        for (Entity laser : cm.getEntitiesWithComponent<LaserBoltComponent>())
        {
            if (laserbolt2spawntime.find(laser) == laserbolt2spawntime.end())
            {
                laserbolt2spawntime[laser] = time.totalTime();
            }
        }

        // Destroy laser bolts that outlived their maximum lifetime.
        for (auto it = laserbolt2spawntime.begin(); it != laserbolt2spawntime.end();)
        {
            Entity laser = it->first;
            float spawnTime = it->second;

            if (time.totalTime() - spawnTime >= maxAliveTime)
            {
                sm.destroyEntity(laser);
                it = laserbolt2spawntime.erase(it);
            }
            else
            {
                ++it;
            }
        }

        for (Entity laser : cm.getEntitiesWithComponent<LaserBoltComponent>()){
            LaserBoltComponent laserBoltComponent = cm.getComponent<LaserBoltComponent>(laser);
            LocalTransformComponent& local = cm.getComponent<LocalTransformComponent>(laser);
            WorldTransformComponent& world = cm.getComponent<WorldTransformComponent>(laser);
            const glm::vec3 forward = glm::normalize(glm::vec3(world.matrix[2]));

            local.position += forward * laserBoltComponent.speed * time.deltaTime();
        }
    }
}