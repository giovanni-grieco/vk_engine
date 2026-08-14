#pragma once

#include <vulkan/vulkan.h>

namespace engine{
    class VulkanBuffer{
        public:
            VulkanBuffer();
            ~VulkanBuffer();

            VulkanBuffer(const VulkanBuffer& src) = delete;
            VulkanBuffer& operator=(const VulkanBuffer& src) = delete;

            VulkanBuffer(VulkanBuffer&& src);
            VulkanBuffer& operator=(VulkanBuffer&& src);

        private:
            void init();
            void destroy();
    };
}