#include "vk_surface.hpp"

#include <stdexcept>

namespace engine
{

    VulkanSurface::VulkanSurface(VulkanInstance &instance, Window &window)
    {
        if (glfwCreateWindowSurface(instance.instance, window.window, nullptr, &surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
        this->instance = instance.instance;
    }

    VulkanSurface::~VulkanSurface()
    {
        if (instance != VK_NULL_HANDLE && surface != VK_NULL_HANDLE)
        {
            vkDestroySurfaceKHR(instance, surface, nullptr);
        }
    }
}