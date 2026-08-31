#pragma once

#include "system.hpp"
#include <glm/glm.hpp>

namespace engine
{
    class PointLightSystem : public System
    {
    public:
        void start() override;
        void update() override;

    private:
        float speed = 10.0f;
        float timeBetweenSwitchOfDirection = 1.0f;
        float timeElapsed = 0.0f;
        int direction = -1;
        glm::vec3 pointA{}; 
        glm::vec3 pointB{};
    };
}