#include "vk_texture_manager.hpp"

#include <stdexcept>

namespace engine
{
    VulkanTextureManager::VulkanTextureManager(VulkanDevice &device, VulkanCommandPool &commandPool, VulkanQueues &queues, VkDescriptorSetLayout textureSetLayout)
        : device_(&device), commandPool_(&commandPool), queues_(&queues), textureSetLayout_(textureSetLayout)
    {
    }

    VulkanTextureManager::~VulkanTextureManager()
    {
        for (const auto &entry : id2pool_)
        {
            vkDestroyDescriptorPool(device_->device, entry.second, nullptr);
        }
        id2pool_.clear();
    }

    TextureID VulkanTextureManager::addTexture(const Texture &texture)
    {
        const TextureID id = nextId_++;

        auto it = textures_.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(id),
            std::forward_as_tuple(*device_, *commandPool_, *queues_)).first;

        VulkanTextureImage &image = it->second;
        image.create(static_cast<uint32_t>(texture.texWidth), static_cast<uint32_t>(texture.texHeight));
        if (!texture.pixels.empty())
        {
            image.upload(texture.pixels.data(), texture.pixels.size());
        }

        // Each texture gets its own descriptor set (bound at set 1) so the draw
        // loop can switch textures by swapping the bound set per draw packet.
        VkDescriptorPoolSize poolSize{};
        poolSize.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSize.descriptorCount = 1;

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = 1;

        VkDescriptorPool pool = VK_NULL_HANDLE;
        if (vkCreateDescriptorPool(device_->device, &poolInfo, nullptr, &pool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture descriptor pool!");
        }

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool;
        allocInfo.descriptorSetCount = 1;
        allocInfo.pSetLayouts = &textureSetLayout_;

        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;
        if (vkAllocateDescriptorSets(device_->device, &allocInfo, &descriptorSet) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate texture descriptor set!");
        }

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = image.imageView;
        imageInfo.sampler = image.sampler;

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSet;
        descriptorWrite.dstBinding = 0; // matches layout(set = 1, binding = 0) in the shader
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(device_->device, 1, &descriptorWrite, 0, nullptr);

        TextureDrawInfo info{};
        info.imageView = image.imageView;
        info.sampler = image.sampler;
        info.descriptorSet = descriptorSet;
        id2drawInfo_[id] = info;
        id2pool_[id] = pool;

        return id;
    }

    void VulkanTextureManager::removeTexture(TextureID id)
    {
        id2drawInfo_.erase(id);
        if (textures_.count(id) != 0)
        {
            pendingDelete_.push_back(id);
        }
    }

    void VulkanTextureManager::compact()
    {
        if (pendingDelete_.empty())
        {
            return;
        }

        // The images and descriptor sets may still be referenced by in-flight
        // frames, so wait for everything to finish before destroying them.
        vkDeviceWaitIdle(device_->device);

        for (TextureID id : pendingDelete_)
        {
            auto poolIt = id2pool_.find(id);
            if (poolIt != id2pool_.end())
            {
                vkDestroyDescriptorPool(device_->device, poolIt->second, nullptr);
                id2pool_.erase(poolIt);
            }
            textures_.erase(id);
        }
        pendingDelete_.clear();
    }

    const TextureDrawInfo *VulkanTextureManager::getTextureInfo(TextureID id) const
    {
        const auto it = id2drawInfo_.find(id);
        return it == id2drawInfo_.end() ? nullptr : &it->second;
    }

    bool VulkanTextureManager::hasTexture(TextureID id) const
    {
        return id2drawInfo_.count(id) != 0;
    }
}
