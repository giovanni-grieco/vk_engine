#pragma once

#include <unordered_map>
#include <vulkan/vulkan.h>
#include "geometry/mesh.hpp"

namespace engine{

    class VulkanBufferManager{
        public:
            std::unordered_map<MeshID, VkBuffer> id2buffer;

            
    };

}