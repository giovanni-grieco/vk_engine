#include "vk_descriptor_set_layout.hpp"

namespace engine
{
    VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VkDevice device)
    {
        init(device);
    }

    VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
    {
        destroy();
    }

    void VulkanDescriptorSetLayout::init(VkDevice device)
    {
        this->device = device;

        // Set 0: the per-frame uniform buffer object.
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo uboLayoutInfo{};
        uboLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        uboLayoutInfo.bindingCount = 1;
        uboLayoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(device, &uboLayoutInfo, nullptr, &uboSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create UBO descriptor set layout!");
        }

        // Set 1: a per-texture combined image sampler.
        VkDescriptorSetLayoutBinding samplerLayoutBinding{};
        samplerLayoutBinding.binding = 0;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo samplerLayoutInfo{};
        samplerLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        samplerLayoutInfo.bindingCount = 1;
        samplerLayoutInfo.pBindings = &samplerLayoutBinding;

        if (vkCreateDescriptorSetLayout(device, &samplerLayoutInfo, nullptr, &textureSetLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture descriptor set layout!");
        }
    }

    void VulkanDescriptorSetLayout::destroy()
    {
        if (device != VK_NULL_HANDLE)
        {
            if (uboSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(device, uboSetLayout, nullptr);
            }
            if (textureSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(device, textureSetLayout, nullptr);
            }
            device = VK_NULL_HANDLE;
            uboSetLayout = VK_NULL_HANDLE;
            textureSetLayout = VK_NULL_HANDLE;
        }
    }
}