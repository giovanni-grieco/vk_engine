#pragma once

#include "ecs/entity/entity.hpp"

#include <vector>

namespace engine{
    class EntityManager{
        public:
            Entity createEntity();
            void destroyEntity(Entity entity);
            const std::vector<Entity>& getActiveEntities() const;

        private:
            static unsigned int nextEntityId;
            std::vector<Entity> entities{};
            std::vector<int> entityToIndex{MAX_ENTITIES, -1};

    };
}