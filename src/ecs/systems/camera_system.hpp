#pragma once
#include "system.hpp"

namespace engine{
    class CameraSystem : public System{
        public:
            CameraSystem(float followDistance=5.0f, float followHeight=2.0f);
            void start() override;
            void update() override;
            
        private:
            float followDistance = 8.0f;
            float followHeight = 2.5f;
    };
}