#pragma once
#include "system.hpp"

namespace engine
{
    class CameraSystem : public System
    {
    public:
        CameraSystem(float speed);

        void start() override;
        void update() override;

    private:
        float speed;
    };
}