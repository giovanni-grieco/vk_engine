#pragma once

#include "system.hpp"

namespace engine{
    class Translator : public System{
        public:
            Translator(float speed);

            void start() override;
            void update() override;
            
            float speed;
    };
}