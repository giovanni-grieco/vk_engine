#include "translator_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"

namespace engine{
    void Translator::start(){
        // Implementazione del metodo start per il sistema di traduzione
    }

    void Translator::update(){
        for (Transform& transform : ComponentManager::getInstance().getComponentArray<Transform>().getComponents()){
            transform.position.x = transform.position.x + 0.1;
        }
    }
}