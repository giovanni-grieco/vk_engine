#pragma once

#include <vector>
#include "entity/entity.hpp"
#include <iostream>

namespace engine
{

    template <typename T>
    class ComponentArray
    {
    public:
        T& getComponent(const Entity entity)
        {
            int position = findComponentIndex(entity);
            return components[position];
        }

        void addComponent(const Entity entity, const T &component)
        {
            components.push_back(component);
            entities.push_back(entity);
        }

        T removeComponent(Entity entity)
        {
            int position = findComponentIndex(entity);
            std::cout << "Removing component for entity " << entity << " at position " << position << "\n";
            T deletedComponent = components[position];
            Entity deletedEntity = entities[position];

            deletedComponent.dump();

            T lastComponent = components[components.size() - 1];
            Entity lastEntity = entities[entities.size() - 1];

            components[position] = lastComponent;

            entities[position] = lastEntity;

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
            for (auto c : components)
            {
                std::cout << "\n\t";
                c.dump();
            }
            std::cout << "\n";
            std::cout <<"=====================\n";
        }

    private:
        std::vector<T> components{};    // List of components
        std::vector<Entity> entities{}; // List of entities which are using this component

        int findComponentIndex(const Entity entity) const
        {
            int i;
            bool found = false;
            for (i = 0; i < entities.size() && !found; i++)
            {
                if (entities[i] == entity)
                {
                    found = true;
                }
            }
            if (found)
            {
                return i-1;
            }
            else
            {
                throw std::out_of_range("Component not found for entity " + std::to_string(entity));
            }
        }
    };
}