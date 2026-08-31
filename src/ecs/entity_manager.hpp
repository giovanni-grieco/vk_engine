#pragma once

#include "ecs/entity/entity.hpp"
#include "ecs/component_manager.hpp"

#include <vector>
#include <memory>

namespace engine{
    class EntityManager{
        public:
            static EntityManager& getInstance();

            EntityManager()=default;

            bool exists(Entity entity);
            const std::vector<Entity>& getActiveEntities() const;

        private:
            friend class SceneManager;

            Entity createEntity();
            void destroyEntity(Entity entity);
            void destroyEntity(Entity entity, ComponentManager& cm);

            static std::unique_ptr<EntityManager> instance;
            static unsigned int nextEntityId;

            EntityManager(const EntityManager&)=delete;
            EntityManager& operator=(const EntityManager&)=delete;

            std::vector<Entity> entities{};
            std::vector<int> entityToIndex = std::vector<int>(MAX_ENTITIES, -1);
            std::vector<Entity> freeIds{};

    };
}