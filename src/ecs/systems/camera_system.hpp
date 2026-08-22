#pragma once
#include "system.hpp"
#include "ecs/entity/entity.hpp"

namespace engine
{
    class CameraSystem : public System
    {
    public:
        CameraSystem(float translationSpeed, float angularSpeed);

        void start() override;
        void update() override;

    private:
        float translationSpeed;
        float angularSpeed;
        void handleInputs(Entity camera);
    };
}