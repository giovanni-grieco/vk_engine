#include "renderer.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/transform.hpp"
#include "frame_info.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace engine
{

    Renderer::Renderer(Window &window, VulkanBackend &vulkanBackend)
        : window(window), vulkan(vulkanBackend)
    {
    }

    FrameInfo Renderer::frameInfoFromCamera(CameraComponent &camera)
    {
        FrameInfo frameInfo{};
        frameInfo.view = glm::lookAt(camera.position, camera.forward, camera.up);
        frameInfo.projection = glm::perspective(
            glm::radians(camera.fov), 
            (vulkan.swapChain.extent.width) / (float)(vulkan.swapChain.extent.height), 
            camera.nearPlane, camera.farPlane
        );
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

            DrawPacket packet{};
            packet.meshId = mesh.meshHandle;
            packet.model = model;
            result.push_back(packet);
        }

        return result;
    }

    void Renderer::render()
    {
        ComponentManager &cm = ComponentManager::getInstance();
        CameraComponent camera = cm.getComponent<CameraComponent>(cm.getEntitiesWithComponent<CameraComponent>()[0]);
        FrameInfo frameInfo = frameInfoFromCamera(camera);
        std::vector<DrawPacket> drawPackets = makeDrawPackets();

        vulkan.submitDrawPackets(drawPackets);
        vulkan.drawFrame(window, frameInfo);
    }
}