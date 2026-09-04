#pragma once

#include "system.hpp"
#include "ecs/entity/entity.hpp"
#include "texture/texture.hpp"
#include "geometry/mesh.hpp"

namespace engine
{
    class PlayerShipSystem : public System
    {
    public:
        struct Ctx
        {
            MeshID laserBoltMeshHandle = -1;
            TextureID laserBoltTexHandle = -1;
        };
        static constexpr float MIN_SPEED = 0.0f;
        static constexpr float MAX_SPEED = 10.0f;
        static constexpr float THROTTLE_RATE = 10.0f;
        static constexpr float DEFAULT_ANGULAR_SPEED = 0.2f;

        PlayerShipSystem(float minSpeed, float maxSpeed, float angularSpeed, Ctx context);

        void start() override;
        void update() override;

    private:
        float minSpeed = MIN_SPEED;
        float currentSpeed = MIN_SPEED;
        float maxSpeed = MAX_SPEED;
        float angularSpeed = DEFAULT_ANGULAR_SPEED;

        float lastTimeFired = 0.0f;
        float timeBetweenFires = 0.175f;

        TextureID laserBoltTexHandle = -1;
        MeshID laserBoltMeshHandle = -1;

        void shootLaser(Entity ship);
        void handleInputs(Entity ship);
    };
}
