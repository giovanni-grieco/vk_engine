#include "free_camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/camera.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"
#include "utils/angles.hpp"

#include <glm/glm.hpp>

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

        CameraComponent &cameraComponent = cm.getComponent<CameraComponent>(camera);
        Input &input = Input::getInstance();

        float dt = EngineTime::getInstance().deltaTime();

        
        this->translationSpeed = updateTranslationSpeed(this->translationSpeed, input.scrollDelta().y);



        if (input.isKeyDown(Key::W))
        {
            cameraComponent.position += cameraComponent.forward * translationSpeed * dt;
        }
        if (input.isKeyDown(Key::S))
        {
            cameraComponent.position -= cameraComponent.forward * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::A))
        {
            // go left
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            cameraComponent.position -= right * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::D))
        {
            // go right
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            cameraComponent.position += right * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::Q))
        {
            // go up
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            glm::vec3 up = glm::normalize(glm::cross(right, cameraComponent.forward));
            cameraComponent.position += up * translationSpeed * dt;
        }

        if (input.isKeyDown(Key::E))
        {
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            glm::vec3 up = glm::normalize(glm::cross(right, cameraComponent.forward));
            cameraComponent.position -= up * translationSpeed * dt;
        }

        // Rotation amount for this frame, in degrees.
        const float angle = angularSpeed * dt;

        // Camera right vector, recomputed every frame from the current basis.
        const glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));

        // Artificial clamp: keep forward within 1..179 degrees of the FIXED world
        // up (i.e. pitch within +/-89 of horizontal) so `right` never degenerates.

        if (input.isKeyDown(Key::Up))
        { // pitch up: rotate the whole basis around `right`
            const glm::vec3 candidate = rotateVector(cameraComponent.forward, right, angle);
            cameraComponent.forward = candidate;
            cameraComponent.up = rotateVector(cameraComponent.up, right, angle);
        }

        if (input.isKeyDown(Key::Down))
        { // pitch down
            const glm::vec3 candidate = rotateVector(cameraComponent.forward, right, -angle);
            cameraComponent.forward = candidate;
            cameraComponent.up = rotateVector(cameraComponent.up, right, -angle);
        }

        if (input.isKeyDown(Key::Right))
        { // yaw right: rotate the whole basis around world up
            cameraComponent.forward = rotateVector(cameraComponent.forward, worldUp, -angle);
            cameraComponent.up = rotateVector(cameraComponent.up, worldUp, -angle);
        }

        if (input.isKeyDown(Key::Left))
        { // yaw left
            cameraComponent.forward = rotateVector(cameraComponent.forward, worldUp, angle);
            cameraComponent.up = rotateVector(cameraComponent.up, worldUp, angle);
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