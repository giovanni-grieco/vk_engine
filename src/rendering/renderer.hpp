#pragma once

#include "ecs/components/camera.hpp"
#include "frame_info.hpp"
#include "vulkan/vk_backend.hpp"

namespace engine{
    class Renderer{
        public:
            Renderer(VulkanBackend* vulkanBackend);
            void render();
            FrameInfo frameInfoFromCamera(CameraComponent& camera);
        
        private:
            VulkanBackend* vulkanBackend;
    };
}