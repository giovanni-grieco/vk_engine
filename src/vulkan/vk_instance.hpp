#pragma once

#include <vulkan/vulkan.h>
#include <string>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <map>
#include <string.h>

#include "platform/window.hpp"

namespace engine
{


    class VulkanInstance
    {
    public:
        VkInstance instance;
        VulkanInstance(std::string applicationName, Window& window, bool enableValidationLayers, const std::vector<const char*>& validationLayers);
        ~VulkanInstance();
        
    private:
        bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
    };
}