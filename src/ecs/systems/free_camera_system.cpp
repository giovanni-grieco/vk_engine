#include "free_camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"

#include <glm/glm.hpp>

#include <algorithm>
#include <iostream>

namespace engine
{
    FreeCameraSystem::FreeCameraSystem(float translationSpeed, float angularSpeed)
    {
        this->translationSpeed = translationSpeed;
        this->angularSpeed = angularSpeed;
    }

    void FreeCameraSystem::start() {}

    float updateTranslationSpeed(float currentSpeed, float delta){
        
        float candidateSpeed = currentSpeed+delta;

        if(delta > 0){
            candidateSpeed = std::min(candidateSpeed, FreeCameraSystem::MAX_TRANS_SPEED);
        }else if(delta < 0){
            candidateSpeed = std::max(candidateSpeed, FreeCameraSystem::MIN_TRANS_SPEED);
        }

        return candidateSpeed;
    }

    void FreeCameraSystem::handleInputs(Entity camera)
    {
        ComponentManager &cm = ComponentManager::getInstance();

        LocalTransformComponent &local = cm.getComponent<LocalTransformComponent>(camera);
        const WorldTransformComponent &world = cm.getComponent<WorldTransformComponent>(camera);
        Input &input = Input::getInstance();

        // Camera basis from the previous frame's world matrix. The camera's
        // local +Z is its forward direction, +Y is up, so right is forward x up.
        const glm::vec3 forward = glm::normalize(glm::vec3(world.matrix[2]));
        const glm::vec3 up = glm::normalize(glm::vec3(world.matrix[1]));
        const glm::vec3 right = glm::normalize(glm::cross(forward, up));

        float dt = EngineTime::getInstance().deltaTime();

        
        this->translationSpeed = updateTranslationSpeed(this->translationSpeed, input.scrollDelta().y);



        if (input.isKeyDown(Key::W))
        {
            local.position += forward * translationSpeed * dt;
        }
        if (input.isKeyDown(Key::S))
        {
            local.position -= forward * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::A))
        {
            // go left
            local.position -= right * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::D))
        {
            // go right
            local.position += right * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::Q))
        {
            // go up
            local.position += up * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::E))
        {
            local.position -= up * translationSpeed * dt;
        }

        // Rotation this frame, in degrees (LocalTransformComponent stores Euler degrees).
        const float angle = angularSpeed * dt;

        // Pitch around the local X axis. Positive X-rotation drops the nose,
        // so pitch up is negative.
        if (input.isKeyDown(Key::Up))
        {
            local.rotation.x -= angle;
        }
        if (input.isKeyDown(Key::Down))
        {
            local.rotation.x += angle;
        }

        // Yaw around the world Y axis. Positive Y-rotation steers left.
        if (input.isKeyDown(Key::Right))
        {
            local.rotation.y -= angle;
        }
        if (input.isKeyDown(Key::Left))
        {
            local.rotation.y += angle;
        }
    }

    void FreeCameraSystem::update()
    {

        ComponentManager &cm = ComponentManager::getInstance();

        auto cameras = cm.getEntitiesWithComponent<CameraComponent>();
        if (cameras.size() > 1)
        {
            std::cout << "More than one camera! returning...\n";
            return;
        }
        else if (cameras.size() < 1)
        {
            std::cout << "No camera exists! returning...\n";
            return;
        }

        Entity cameraEntity = cameras[0];
        handleInputs(cameraEntity);
    }
}