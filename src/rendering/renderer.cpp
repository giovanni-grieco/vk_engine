#include "renderer.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/transform.hpp"
#include "ecs/components/texture.hpp"
#include "frame_info.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace engine
{

    Renderer::Renderer(Window &window, VulkanBackend &vulkanBackend)
        : window(window), vulkan(vulkanBackend)
    {
    }

    FrameInfo Renderer::demoFrameInfo()
    {
        FrameInfo frameInfo{};
        frameInfo.view = glm::lookAt(glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        frameInfo.projection = glm::perspective(glm::radians(45.0f), vulkan.swapChain.extent.width / (float)vulkan.swapChain.extent.height, 0.1f, 10.0f);
        frameInfo.projection[1][1] *= -1;
        return frameInfo;
    }

    FrameInfo Renderer::frameInfoFromCamera(CameraComponent &camera)
    {
        FrameInfo frameInfo{};
        frameInfo.view = glm::lookAt(camera.position, camera.position + camera.forward, camera.up);
        frameInfo.projection = glm::perspective(
            glm::radians(camera.fov), 
            (vulkan.swapChain.extent.width) / (float)(vulkan.swapChain.extent.height), 
            camera.nearPlane, camera.farPlane
        );
        frameInfo.projection[1][1] *= -1;
        return frameInfo;
    }

    std::vector<DrawPacket> makeDrawPackets()
    {
        ComponentManager &cm = ComponentManager::getInstance();
        std::vector<DrawPacket> result;
        auto entitiesWithMesh = cm.getEntitiesWithComponent<MeshComponent>();
        for(auto entity : entitiesWithMesh){
            MeshComponent& mesh = cm.getComponent<MeshComponent>(entity);
            TransformComponent& transform = cm.getComponent<TransformComponent>(entity);

            glm::mat4 model = glm::translate(glm::mat4(1.0f), transform.position)
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
                            * glm::rotate(glm::mat4(1.0f), glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
                            * glm::scale(glm::mat4(1.0f), transform.scale);

            glm::mat3 normalMat = glm::transpose(glm::inverse(glm::mat3(model)));

            TextureID textureHandle = -1;
            if(cm.hasComponent<TextureComponent>(entity)){
                TextureComponent& texComp = cm.getComponent<TextureComponent>(entity);
                textureHandle = texComp.textureHandle;
            }

            DrawPacket packet{};
            packet.meshId = mesh.meshHandle;
            packet.textureId=textureHandle;
            packet.model = model;
            packet.normal = glm::mat4(normalMat);
            result.push_back(packet);
        }

        return result;
    }

    void Renderer::render()
    {   
        ComponentManager &cm = ComponentManager::getInstance();
        auto cameras = cm.getEntitiesWithComponent<CameraComponent>();

        FrameInfo frameInfo;

        if(!cameras.empty()){
            auto camera = cm.getComponent<CameraComponent>(cameras[0]);
            frameInfo = frameInfoFromCamera(camera);
        }else{
            frameInfo = demoFrameInfo();
        }

        std::vector<DrawPacket> drawPackets = makeDrawPackets();
        vulkan.submitDrawPackets(drawPackets);

        vulkan.drawFrame(window, frameInfo);
    }
}