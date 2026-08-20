#include "renderer.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "frame_info.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>




namespace engine{

    Renderer::Renderer(VulkanBackend* vulkanBackend){
        this->vulkanBackend = vulkanBackend;
    }

    FrameInfo Renderer::frameInfoFromCamera(CameraComponent& camera){
        FrameInfo frameInfo{};
        frameInfo.view = glm::lookAt(camera.position, camera.forward, camera.up);
        frameInfo.projection = glm::perspective(glm::radians(camera.fov), (vulkanBackend->swapChain.extent.width) / (float) (vulkanBackend->swapChain.extent.height), camera.nearPlane, camera.farPlane);
        return frameInfo;
    }

    std::vector<DrawPacket> makeDrawPackets(std::vector<Entity> entities){
        
        std::vector<DrawPacket> result;
        // implementare la logica per creare la model matrix
        // Prendere transform e utilizzarla per creare una model matrix
        
        return result;
    }    

    void Renderer::render(){
        ComponentManager& cm = ComponentManager::getInstance(); 
        std::vector<Entity> entities = cm.getEntitiesWithComponent<MeshComponent>();
        CameraComponent camera = cm.getComponent<CameraComponent>(cm.getEntitiesWithComponent<CameraComponent>()[0]);
        FrameInfo frameInfo = frameInfoFromCamera(camera);

    }
}