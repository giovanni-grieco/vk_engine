#pragma once

#include "system.hpp"

namespace engine{
    class TransformSystem : public System{
    public:
        void start() override;
        void update() override;
    };
}