#pragma once

#include "system.hpp"

namespace engine{
    class TransformSystem : public System{
        void start() override;
        void update() override;
    };
}