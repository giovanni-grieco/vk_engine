#include "camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/camera.hpp"
#include "input/input_system.hpp"
#include "time/engine_time.hpp"

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
            //The vector obtained via cross product of up and forward
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            cameraComponent.position -= right * translationSpeed * dt;
        }

        if(input.isKeyPressed(68)){
            //go right
            //std::cout<<"Camera right\n";
            glm::vec3 right = glm::normalize(glm::cross(cameraComponent.forward, cameraComponent.up));
            cameraComponent.position += right * translationSpeed * dt;
        }


        if(input.isKeyPressed(81)){
            //go up
            glm::vec3 up = cameraComponent.up;
            cameraComponent.position += up * translationSpeed * dt;
        }
        
        if(input.isKeyPressed(69)){
            glm::vec3 up = cameraComponent.up;
            cameraComponent.position -= up * translationSpeed * dt;
        }


        if(input.isKeyPressed(265)){
            //camera rotate up
            std::cout<<"Camera rotate up\n";
        }

        if(input.isKeyPressed(262)){
            //camera rotate right
            std::cout<<"Camera rotate right\n";
        }

        if(input.isKeyPressed(264)){
            std::cout<<"Camera rotate down\n";
        }

        if(input.isKeyPressed(263)){
            std::cout<<"Camera rotate left\n";
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