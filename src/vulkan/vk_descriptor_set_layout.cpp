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

    void VulkanDescriptorSetLayout::init(VkDevice device){
        this->device = device;
        VkDescriptorSetLayoutBinding uboLayoutBinding{};
        uboLayoutBinding.binding = 0;
        uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        uboLayoutBinding.descriptorCount = 1;
        uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
        uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

        VkDescriptorSetLayoutCreateInfo layoutInfo{};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &uboLayoutBinding;

        if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &layout) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create descriptor set layout!");
        }
    }

    void VulkanDescriptorSetLayout::destroy(){
        if(device != VK_NULL_HANDLE && layout != VK_NULL_HANDLE){
            vkDestroyDescriptorSetLayout(device, layout, nullptr);
            device = VK_NULL_HANDLE;
            layout = VK_NULL_HANDLE;
        }
    }
}