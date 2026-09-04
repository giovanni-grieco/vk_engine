#pragma once

#include "ecs/components/camera.hpp"
#include "ecs/components/world_transform.hpp"
#include "frame_info.hpp"
#include "vulkan/vk_backend.hpp"
#include "platform/window.hpp"

namespace engine
{
    class Renderer
    {
    public:
        Renderer(Window &window, VulkanBackend &vulkanBackend);
        void render();
        FrameInfo frameInfoFromCamera(const WorldTransformComponent &world, const CameraComponent &camera);

    private:
        Window &window;
        VulkanBackend &vulkan;

        FrameInfo demoFrameInfo();
    };
}