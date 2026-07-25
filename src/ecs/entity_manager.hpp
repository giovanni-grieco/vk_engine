#pragma once

#include "ecs/entity/entity.hpp"

#include <vector>
#include <memory>

namespace engine{
    class EntityManager{
        public:
            static EntityManager& getInstance();

            Entity createEntity();
            void destroyEntity(Entity entity);
            const std::vector<Entity>& getActiveEntities() const;

        private:
            static std::unique_ptr<EntityManager> instance;
            static unsigned int nextEntityId;

            EntityManager()=default;

            EntityManager(const EntityManager&)=delete;
            EntityManager& operator=(const EntityManager&)=delete;

            std::vector<Entity> entities{};
            std::vector<int> entityToIndex{MAX_ENTITIES, -1};

    };
}