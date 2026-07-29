#include "entity_manager.hpp"
#include "ecs/component_manager.hpp"
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
        if (!freeIds.empty())
        {
            Entity recycled = freeIds.back();

            freeIds.pop_back();

            entities.push_back(recycled);

            entityToIndex[recycled] = entities.size()-1;

            return recycled;
        }

        if (nextEntityId >= MAX_ENTITIES) {
            std::cerr << "Error: Maximum entity count (" << MAX_ENTITIES << ") reached.\n";
            return MAX_ENTITIES; // sentinel — caller must check
        }
        Entity newEntity = nextEntityId++;
        entityToIndex[newEntity] = entities.size();
        entities.push_back(newEntity);
        return newEntity;
    }

    void EntityManager::destroyEntity(Entity entity)
    {
        destroyEntity(entity, ComponentManager::getInstance());
    }

    void EntityManager::destroyEntity(Entity entity, ComponentManager& cm){
        auto index = entityToIndex[entity];
        if (index == -1){
            std::cerr << "Error: Attempting to destroy an entity that does not exist: " << entity << "\n";
            return;
        }

        // Remove all components associated with this entity
        cm.removeEntity(entity);

        Entity lastEntity = entities.back();
        entities[index] = lastEntity;
        entityToIndex[lastEntity] = index;
        entities.pop_back();
        entityToIndex[entity] = -1;
        // Recycle the ID
        freeIds.push_back(entity);
    }

    bool EntityManager::exists(Entity entity) {
        return entityToIndex[entity]!=-1;
    }

    const std::vector<Entity>& EntityManager::getActiveEntities() const
    {
        return entities;
    }

}