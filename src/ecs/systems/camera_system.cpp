#include "camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/camera.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"
#include "utils/angles.hpp"

#include <glm/glm.hpp>

#include <iostream>

namespace engine
{
    CameraSystem::CameraSystem(float translationSpeed, float angularSpeed)
    {
        this->translationSpeed = translationSpeed;
        this->angularSpeed = angularSpeed;
    }

    void CameraSystem::start() {}

    void CameraSystem::handleInputs(Entity camera)
    {
        ComponentManager &cm = ComponentManager::getInstance();

        CameraComponent &cameraComponent = cm.getComponent<CameraComponent>(camera);
        Input& input = Input::getInstance();

        float dt = EngineTime::getInstance().deltaTime();

        if (input.isKeyPressed(87)){
            cameraComponent.position += cameraComponent.forward * translationSpeed * dt;
        }
        if(input.isKeyPressed(83)){
            cameraComponent.position -= cameraComponent.forward * translationSpeed * dt;
        }

        if(input.isKeyPressed(65)){
            //go left
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, worldUp));
            cameraComponent.position -= right * translationSpeed * dt;
        }

        if(input.isKeyPressed(68)){
            //go right
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, worldUp));
            cameraComponent.position += right * translationSpeed * dt;
        }


        if(input.isKeyPressed(81)){
            //go up
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, worldUp));
            glm::vec3 up = glm::normalize(glm::cross(right, cameraComponent.forward));
            cameraComponent.position += up * translationSpeed * dt;
        }
        
        if(input.isKeyPressed(69)){
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, worldUp));
            glm::vec3 up = glm::normalize(glm::cross(right, cameraComponent.forward));
            cameraComponent.position -= up * translationSpeed * dt;
        }

        // Rotation amount for this frame, in degrees.
        const float angle = angularSpeed * dt;

        // Camera right vector, recomputed every frame from the current basis.
        const glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));

        // Artificial clamp: keep forward within 1..179 degrees of the FIXED world
        // up (i.e. pitch within +/-89 of horizontal) so `right` never degenerates.
        constexpr float kMinAngleToWorldUp = 1.0f;
        constexpr float kMaxAngleToWorldUp = 179.0f;

        if(input.isKeyPressed(265)){ // pitch up: rotate the whole basis around `right`
            const glm::vec3 candidate = rotateVector(cameraComponent.forward, right, angle);
            if (angleBetween(candidate, worldUp) >= kMinAngleToWorldUp)
            {
                cameraComponent.forward = candidate;
                cameraComponent.up = rotateVector(cameraComponent.up, right, angle);
            }
        }

        if(input.isKeyPressed(264)){ // pitch down
            const glm::vec3 candidate = rotateVector(cameraComponent.forward, right, -angle);
            if (angleBetween(candidate, worldUp) <= kMaxAngleToWorldUp)
            {
                cameraComponent.forward = candidate;
                cameraComponent.up = rotateVector(cameraComponent.up, right, -angle);
            }
        }

        if(input.isKeyPressed(262)){ // yaw right: rotate the whole basis around world up
            cameraComponent.forward = rotateVector(cameraComponent.forward, worldUp, -angle);
            cameraComponent.up = rotateVector(cameraComponent.up, worldUp, -angle);
        }

        if(input.isKeyPressed(263)){ // yaw left
            cameraComponent.forward = rotateVector(cameraComponent.forward, worldUp, angle);
            cameraComponent.up = rotateVector(cameraComponent.up, worldUp, angle);
        }

    }

    void CameraSystem::update()
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