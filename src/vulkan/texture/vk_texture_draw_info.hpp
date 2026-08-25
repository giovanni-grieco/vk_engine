#pragma once

#include <vulkan/vulkan.h>

namespace engine
{
    // GPU-side handles for a texture that get bound to a combined image sampler.
    struct TextureDrawInfo
    {
        VkImageView imageView = VK_NULL_HANDLE;
        VkSampler sampler = VK_NULL_HANDLE;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE; // set 1, bound per draw
    };
}
