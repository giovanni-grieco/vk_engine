#pragma once

#include "system.hpp"
#include "ecs/entity/entity.hpp"
#include <unordered_map>
#include <chrono>

namespace engine
{
    class LaserSystem : public System
    {
        public:
            void start() override;
            void update() override;
        private:
            float maxAliveTime = 5.0f;
            std::unordered_map<Entity, float> laserbolt2spawntime;
    };
}