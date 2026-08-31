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

        // Set 0: the per-frame uniform buffer object (binding 0) and the
        // point-light storage buffer (binding 1).
        VkDescriptorSetLayoutBinding bindings[2]{};

        bindings[0].binding = 0;
        bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        bindings[0].descriptorCount = 1;
        bindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        bindings[0].pImmutableSamplers = nullptr; // Optional

        bindings[1].binding = 1;
        bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        bindings[1].descriptorCount = 1;
        bindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
        bindings[1].pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo uboLayoutInfo{};
        uboLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        uboLayoutInfo.bindingCount = 2;
        uboLayoutInfo.pBindings = bindings;

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