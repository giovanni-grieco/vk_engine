#pragma once

#include <vector>
#include "ecs/entity/entity.hpp"
#include <iostream>

namespace engine
{

    template <typename T>
    class ComponentArray
    {
    public:

        ComponentArray() : entityToIndex(MAX_ENTITIES, -1) {};

        T& getComponent(const Entity entity)
        {
            int position = findComponentIndex(entity);
            return components[position];
        }

        void addComponent(const Entity entity, const T &component)
        {
            components.push_back(component);
            entities.push_back(entity);
            entityToIndex[entity] = components.size() - 1;
        }

        T removeComponent(Entity entity)
        {
            int position = findComponentIndex(entity);
            T deletedComponent = components[position];
            Entity deletedEntity = entities[position];
            entityToIndex[deletedEntity] = -1;


            T lastComponent = components[components.size() - 1];
            Entity lastEntity = entities[entities.size() - 1];

            components[position] = lastComponent;

            entities[position] = lastEntity;

            entityToIndex[lastEntity] = position;

            components.pop_back();

            entities.pop_back();

            return deletedComponent;
        }

        const std::vector<T>& rawData() const
        {
            return components;
        }

        const std::vector<Entity>& rawEntities() const
        {
            return entities;
        }

        void dump() const
        {
            std::cout << "ComponentArray dump:\n";
            std::cout << "Components (" << components.size() << ")\n";
            std::cout << "Entities: ";
            for (auto e : entities)
                std::cout << e << " ";
            std::cout << "\n";
            std::cout <<"=====================\n";
        }

    private:
        std::vector<T> components{};    // List of components
        std::vector<Entity> entities{}; // List of entities which are using this component
        std::vector<int> entityToIndex{}; // List of indices of the components for each entity

        int findComponentIndex(const Entity entity) const
        {
            int index = entityToIndex[entity];
            return index;
        }
    };
}