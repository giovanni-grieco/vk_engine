#pragma once

#include "system.hpp"

namespace engine{
    class PointLightSystem: public System{
        public:
            void start() override;
            void update() override;
    };
}