#include "free_camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/local_transform.hpp"
#include "ecs/components/world_transform.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

        const float angle = glm::radians(angularSpeed * dt);

        if (input.isKeyDown(Key::Up))
        {
            local.rotation = local.rotation * glm::angleAxis(-angle, glm::vec3(1.0f, 0.0f, 0.0f));
        }
        if (input.isKeyDown(Key::Down))
        {
            local.rotation = local.rotation * glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        // Yaw around the local Y axis. Positive Y-rotation steers left.
        if (input.isKeyDown(Key::Right))
        {
            local.rotation = local.rotation * glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
        }
        if (input.isKeyDown(Key::Left))
        {
            local.rotation = local.rotation * glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        local.rotation = glm::normalize(local.rotation);
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