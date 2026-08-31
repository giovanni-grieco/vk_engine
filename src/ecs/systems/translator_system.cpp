#include "translator_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/world_transform.hpp"

#include "time/engine_time.hpp"

namespace engine{

    Translator::Translator(float speed){
        this->speed = speed;
    }

    void Translator::start(){
        // Implementazione del metodo start per il sistema di traduzione
    }

    void Translator::update(){
        
    }
}