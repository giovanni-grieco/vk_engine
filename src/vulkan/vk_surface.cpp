#include "vk_surface.hpp"

#include <stdexcept>

namespace engine{
    
    VulkanSurface::VulkanSurface(VkInstance instance, Window& window){
        if (glfwCreateWindowSurface(instance, window.window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
        this->instance = instance;
    }

    VulkanSurface::~VulkanSurface(){
        if (surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
    }
}