#pragma once
#include "system.hpp"
#include "ecs/entity/entity.hpp"

namespace engine
{
    class CameraSystem : public System
    {
    public:
        static constexpr float MAX_TRANS_SPEED = 50.0f;
        static constexpr float MIN_TRANS_SPEED = 1.0f;

        CameraSystem(float translationSpeed, float angularSpeed);

        void start() override;
        void update() override;

    private:
        float translationSpeed;
        float angularSpeed;
        void handleInputs(Entity camera);
    };
}