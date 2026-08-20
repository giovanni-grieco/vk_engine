#include "camera_system.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/camera.hpp"
#include "input/input_system.hpp"

namespace engine
{
    CameraSystem::CameraSystem(float speed){
        this->speed = speed;
    }

    void CameraSystem::start(){}

    void CameraSystem::update(){

        ComponentManager& cm = ComponentManager::getInstance();
        
        auto cameras = cm.getEntitiesWithComponent<CameraComponent>();
        if (cameras.size() > 1){
            //std::cout<< "More than one camera! returning...\n";
            return;
        }else if(cameras.size() < 1){
            //std::cout<< "No camera exists! returning...\n";
            return;
        }
        
        Entity cameraEntity = cameras[0];

        TransformComponent& transform = cm.getComponent<TransformComponent>(cameraEntity);

        //we update the transform based on the input system;


    }
}