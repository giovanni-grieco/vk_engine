#include "entity_manager.hpp"
#include <iostream>

namespace engine
{
    std::unique_ptr<EntityManager> EntityManager::instance = nullptr;

    unsigned int EntityManager::nextEntityId = 0;

    EntityManager& EntityManager::getInstance(){
        if(instance==nullptr){
            instance=std::unique_ptr<EntityManager>(std::make_unique<EntityManager>());
        }
        return *instance;
    }

    Entity EntityManager::createEntity()
    {
        Entity newEntity = nextEntityId++;
        entities.push_back(newEntity);
        return newEntity;
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        auto index = entityToIndex[entity];
        if (index == -1){
            std::cerr << "Error: Attempting to destroy an entity that does not exist: " << entity << "\n";
            return;
        }

        Entity lastEntity = entities.back();
        entities[index] = lastEntity;
        entityToIndex[lastEntity] = index;
        entities.pop_back();
        entityToIndex[entity] = -1;
    }

    const std::vector<Entity>& EntityManager::getActiveEntities() const
    {
        return entities;
    }

}