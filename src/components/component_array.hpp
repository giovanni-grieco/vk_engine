#pragma once

#include <vector>

#include "entity/entity.hpp"

namespace engine{
    
    template<typename T>
    class ComponentArray {
        public:
            T& getComponent(Entity entity);
            void addComponent(Entity entity, T& component); 
            T& removeComponent(Entity entity);
            std::vector<T>& data();
        private:
            std::vector<T> components{}; //List of components
            std::vector<Entity> entityList{}; //List of entities which are using this component
            std::vector<int> indexList{}; //List of the corresponding component indexes for each entity
            // for example entityList[3] has its corresponding component inside components[indexList[3]]
    };
}