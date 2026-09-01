#include "renderer.hpp"
#include "ecs/component_manager.hpp"
#include "ecs/components/mesh.hpp"
#include "ecs/components/camera.hpp"
#include "ecs/components/world_transform.hpp"
#include "ecs/components/texture.hpp"
#include "ecs/components/point_light.hpp"
#include "ecs/components/ambient_light.hpp"
#include "ecs/components/directional_light.hpp"
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

            glm::mat4 model = cm.getComponent<WorldTransformComponent>(entity).matrix;

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

    LightInfo makeLightInfo()
    {
        ComponentManager &cm = ComponentManager::getInstance();
        LightInfo data{};
        for(auto entity : cm.getEntitiesWithComponent<PointLightComponent>()){
            if(data.pointLights.size() >= MAX_POINT_LIGHTS)
                break;

            PointLightComponent &light = cm.getComponent<PointLightComponent>(entity);
            glm::mat4 &world = cm.getComponent<WorldTransformComponent>(entity).matrix;

            data.pointLights.push_back(PointLightInfo{glm::vec3(world[3]), light.color, light.intensity});
        }

        const std::vector<Entity>& ambientLights = cm.getEntitiesWithComponent<AmbientLightComponent>();
        if(ambientLights.size() > 0){
            Entity ambientLight = ambientLights[0];
            AmbientLightComponent& ambientLightComp = cm.getComponent<AmbientLightComponent>(ambientLight);
            data.ambient.intensity = ambientLightComp.intensity;
            data.ambient.color = ambientLightComp.color;
        }

        const std::vector<Entity>& directionaLights = cm.getEntitiesWithComponent<DirectionalLightComponent>();
        if(directionaLights.size() > 0){
            Entity directionalLight = directionaLights[0];
            DirectionalLightComponent& directionalLightComp = cm.getComponent<DirectionalLightComponent>(directionalLight);
            data.directional.direction = directionalLightComp.direction;
            data.directional.color = directionalLightComp.color;
            data.directional.intensity = directionalLightComp.intensity;
        }


        return data;
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
        vulkan.submitLights(makeLightInfo());

        vulkan.drawFrame(window, frameInfo);
    }
}