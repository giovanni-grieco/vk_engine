#include "translator_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"

#include "time/engine_time.hpp"

namespace engine{

    Translator::Translator(float speed){
        this->speed = speed;
    }

    void Translator::start(){
        // Implementazione del metodo start per il sistema di traduzione
    }

    void Translator::update(){
        for (TransformComponent& transform : ComponentManager::getInstance().getComponentArray<TransformComponent>().getComponents()){
            float dt = EngineTime::getInstance().deltaTime();
            transform.position.x = transform.position.x - (speed * dt);
        }
    }
}