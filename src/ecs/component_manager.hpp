#pragma once

#include "ecs/entity/entity.hpp"
#include "ecs/components/component_array.hpp"

#include <vector>
#include <unordered_map>
#include <typeindex>
#include <stdexcept>
#include <iostream>
#include <memory>

namespace engine
{
    class ComponentManager
    {
    public:

        static ComponentManager& getInstance(){
            if (instance == nullptr){
                instance = std::unique_ptr<ComponentManager>(std::make_unique<ComponentManager>());
            }
            return *instance;
        }

        ComponentManager() = default;

        template <typename T>
        void registerComponent(){
            //controlliamo se non esista già il tipo di componente
            std::type_index typeIndex(typeid(T));
            if (componentArrays.find(typeIndex) != componentArrays.end()){
                std::cerr << "Error: Component type already registered.\n";
                return;
            }
            
            componentArrays[typeIndex] = std::make_unique<ComponentArray<T>>();
        }

        template <typename T>
        void unregisterComponent(){
            std::type_index typeIndex(typeid(T));
            auto it = componentArrays.find(typeIndex);
            if (it != componentArrays.end()){
                componentArrays.erase(it);
            } else {
                std::cerr << "Error: Component type not registered.\n";
            }
        }

        template <typename T>
        void addComponent(Entity entity, const T& component){
                ComponentArray<T>& componentArray = getComponentArray<T>();
                componentArray.addComponent(entity, component);
        }

        template <typename T>
        void addComponent(Entity entity, T&& component){
                ComponentArray<T>& componentArray = getComponentArray<T>();
                componentArray.addComponent(entity, std::move(component));
        }

        /*template <typename T>
        void addComponent(Entity entity, T component){
            ComponentArray<T>& componentArray = getComponentArray<T>();
            componentArray.addComponent(entity, component);
        }*/

        template <typename T>
        void removeComponent(Entity entity){
            ComponentArray<T>& componentArray = getComponentArray<T>();
            componentArray.removeComponent(entity);
        }

        template <typename T>
        T& getComponent(Entity entity){
            ComponentArray<T>& componentArray = getComponentArray<T>();
            return componentArray.getComponent(entity);
        }

        template <typename T>
        bool hasComponent(Entity entity) const{
            const ComponentArray<T>& componentArray = getComponentArrayConst<T>();
            return componentArray.hasComponent(entity);
        }


        template <typename T> //readonly array of entities. We don't want sideways modification!
        const std::vector<Entity>& getEntitiesWithComponent() const{
            const ComponentArray<T>& componentArray = getComponentArrayConst<T>();
            return componentArray.getEntitiesConst();
        }

        void removeEntity(Entity entity)
        {
            for (auto& [type, array] : componentArrays)
            {
                array->removeEntity(entity);
            }
        }

        template<typename T>
        ComponentArray<T>& getComponentArray(){
            auto key = std::type_index(typeid(T));
            auto it = componentArrays.find(key);
            if(it != componentArrays.end()){
                return static_cast<ComponentArray<T>&>(*it->second);
            }
            throw std::runtime_error("Component type not registered.");
        }

        template<typename T>
        const ComponentArray<T>& getComponentArrayConst() const {
            auto key = std::type_index(typeid(T));
            auto it = componentArrays.find(key);
            if(it != componentArrays.end()){
                return static_cast<const ComponentArray<T>&>(*it->second);
            }
            throw std::runtime_error("Component type not registered.");
        }

    private:
        static std::unique_ptr<ComponentManager> instance;

        // Prevent copying — unique_ptr members are non-copyable
        ComponentManager(const ComponentManager&) = delete;
        ComponentManager& operator=(const ComponentManager&) = delete;


        std::unordered_map<std::type_index, std::unique_ptr<IComponentArray>> componentArrays;



    };
}