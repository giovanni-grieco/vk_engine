#pragma once

#include "system.hpp"
#include "ecs/entity/entity.hpp"

namespace engine{
    class PlayerShipSystem:public System{
        public:
            static constexpr float MIN_SPEED = 0.0f;
            static constexpr float MAX_SPEED = 10.0f;
            static constexpr float THROTTLE_RATE = 10.0f;
            static constexpr float DEFAULT_ANGULAR_SPEED = 0.2f;

            PlayerShipSystem(float minSpeed = MIN_SPEED, float maxSpeed = MAX_SPEED, float angularSpeed = DEFAULT_ANGULAR_SPEED);

            void start() override;
            void update() override;

        private:
            float minSpeed = MIN_SPEED;
            float currentSpeed = MIN_SPEED;
            float maxSpeed = MAX_SPEED;
            float angularSpeed = DEFAULT_ANGULAR_SPEED;

            float lastTimeFired = 0.0f;
            float timeBetweenFires = 0.1f;

            void shootLaser(Entity ship);
            void handleInputs(Entity ship);
    };  
}
