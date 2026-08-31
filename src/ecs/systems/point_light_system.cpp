#include "point_light_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/point_light.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/entity/entity.hpp"
#include "time/engine_time.hpp"

namespace engine
{
    void PointLightSystem::start() {}

    void PointLightSystem::update()
    {
        auto &cm = ComponentManager::getInstance();
        auto &time = EngineTime::getInstance();

        const std::vector<Entity> &entities = cm.getEntitiesWithComponent<PointLightComponent>();
        
        for(Entity e : entities){
            LocalTransformComponent& localTransform = cm.getComponent<LocalTransformComponent>(e);

            localTransform.position.x += ((float)direction) * speed * time.deltaTime();
        }

        timeElapsed += time.deltaTime();

        if(timeElapsed >= timeBetweenSwitchOfDirection){
            timeElapsed = 0;
            direction *= -1;
        }
        
    }
}