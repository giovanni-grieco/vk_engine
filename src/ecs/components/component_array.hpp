#pragma once

#include <vector>
#include "ecs/entity/entity.hpp"
#include <iostream>

namespace engine
{

    class IComponentArray
    {
    public:
        virtual ~IComponentArray() = default;
        virtual void removeEntity(Entity entity) = 0;
    };

    template <typename T>
    class ComponentArray :  public IComponentArray
    {
    public:

        ComponentArray<T>() : entityToIndex(MAX_ENTITIES, -1) {};

        T& getComponent(const Entity entity)
        {
            int position = findComponentIndex(entity);
            if (position == -1)
                throw std::runtime_error("Entity does not have this component.");
            return components[static_cast<size_t>(position)];
        }

        void addComponent(const Entity entity, const T &component)
        {
            if (entityToIndex[entity] != -1) {
                std::cerr << "Warning: Entity already has this component — skipping add.\n";
                return;
            }
            std::cout<< "Add Component v1 called\n";
            components.push_back(component);
            entities.push_back(entity);
            entityToIndex[entity] = components.size() - 1;
        }

        void addComponent(const Entity entity, T&& component)
        {
            if (entityToIndex[entity] != -1) {
                std::cerr << "Warning: Entity already has this component — skipping add.\n";
                return;
            }
            std::cout<< "Add Component v2 called\n";
            components.push_back(std::move(component));
            entities.push_back(entity);
            entityToIndex[entity] = components.size() - 1;
        }

        void removeComponent(Entity entity)
        {
            int position = findComponentIndex(entity);
            if (position == -1)
                throw std::runtime_error("Entity does not have this component.");
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
            dump();
            return deletedComponent;
        }

        void removeEntity(Entity entity) override
        {   
            int position = findComponentIndex(entity);
            if (position == -1) return;

            entityToIndex[entity] = -1;

            Entity lastEntity = entities.back();
            
            if (position != static_cast<int>(components.size()) - 1)
            {
                components[position] = std::move(components.back());
                entities[position] = lastEntity;
                entityToIndex[lastEntity] = position;
            }
            components.pop_back();
            entities.pop_back();

        }

        bool hasComponent(Entity entity) const {
            return entityToIndex[entity] != -1;
        }

        std::vector<T>& getComponents()
        {
            return components;
        }

        std::vector<Entity>& getEntities()
        {
            return entities;
        }

        const std::vector<Entity>& getEntitiesConst() const{
            return entities;
        }

        void dump() const
        {
            std::cout << "ComponentArray dump:\n";
            std::cout << "Components size: " << components.size() << "\n";
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