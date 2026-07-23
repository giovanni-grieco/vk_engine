#include "component_array.hpp"

#include "entity/entity.hpp"

namespace engine {

    template<typename T>
    T& ComponentArray<T>::getComponent(Entity entity){

    }

    template<typename T>
    void ComponentArray<T>::addComponent(Entity entity, T& component){
        components.push_back(component);
        indexList.push_back(components.size()-1);
        entityList.push_back(entity);
    }

    template<typename T>
    T& ComponentArray<T>::removeComponent(Entity entity){
        int entityPosition = 0;
        for (auto ent: entityList){
            if (ent == entity){
                break;
            }
            entityPosition +=1;
        }
        
    }

    template<typename T>
    std::vector<T>& ComponentArray<T>::data(){

    }
}