#pragma once

#include "ecs/entity/entity.hpp"
#include <vector>

// Ha la responsabilità di "aggiungere" e "rimuovere" componenti alle entità.
// Quindi detiene tutti i "ComponentArray" in cui sono contenute le struct relative ai componenti e alle loro entità.

// Quando un'entità viene distrutta, il ComponentManager deve rimuovere tutti i componenti associati a quell'entità.
// Questa "notifica" arriverà dall'entity manager, che chiamerà un metodo del ComponentManager per rimuovere i componenti associati all'entità distrutta.

namespace engine{
    class ComponentManager{
        public:

            

        private:
            
    };
}