#pragma once

#include "system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/car.hpp"
#include "ecs/entity/entity.hpp"
#include <iostream>

namespace engine{
    class CarSystem : public System{
        void start() override;
        void update() override;
    };
}