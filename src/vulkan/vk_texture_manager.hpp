#pragma once

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

#include "texture/texture.hpp"
#include "vk_texture_image.hpp"
#include "texture/vk_texture_draw_info.hpp"

namespace engine
{
    // Owns one VulkanTextureImage per live texture and maps TextureID -> TextureDrawInfo.
    class VulkanTextureManager
    {
    public:
        VulkanTextureManager(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues, VkDescriptorSetLayout textureSetLayout);
        ~VulkanTextureManager();

        VulkanTextureManager(const VulkanTextureManager &) = delete;
        VulkanTextureManager &operator=(const VulkanTextureManager &) = delete;

        // Uploads a texture and returns its handle. Handles are assigned sequentially.
        TextureID addTexture(const Texture &texture);

        // Marks a texture as deleted. Its GPU image becomes unreachable until
        // compact() actually destroys it. Safe to call any time.
        void removeTexture(TextureID id);

        // Destroys the GPU resources of removed textures. Stalls the device, so
        // call it when no frames are in flight.
        void compact();

        // GPU draw info for a previously added texture, or nullptr if unknown.
        const TextureDrawInfo *getTextureInfo(TextureID id) const;

        bool hasTexture(TextureID id) const;

    private:
        VulkanDevice *device_;
        VulkanCommandPool *commandPool_;
        VulkanQueues *queues_;
        VkDescriptorSetLayout textureSetLayout_ = VK_NULL_HANDLE;

        std::unordered_map<TextureID, VulkanTextureImage> textures_;
        std::unordered_map<TextureID, TextureDrawInfo> id2drawInfo_;
        std::unordered_map<TextureID, VkDescriptorPool> id2pool_;
        std::vector<TextureID> pendingDelete_;

        TextureID nextId_ = 0;
    };
}
