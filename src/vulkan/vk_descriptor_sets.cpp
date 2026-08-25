#include "vk_descriptor_sets.hpp"
#include <stdexcept>

namespace engine
{
    VulkanDescriptorSets::VulkanDescriptorSets(VulkanDevice &device,
                                               VulkanDescriptorSetLayout &layout,
                                               VulkanDescriptorPool &pool,
                                               VulkanUniformBuffer &uniformBuffer,
                                               uint32_t framesInFlight)
    {
        std::vector<VkDescriptorSetLayout> layouts(framesInFlight, layout.uboSetLayout);

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = pool.descriptorPool;
        allocInfo.descriptorSetCount = framesInFlight;
        allocInfo.pSetLayouts = layouts.data();

        descriptorSets.resize(framesInFlight);
        if (vkAllocateDescriptorSets(device.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS)
            throw std::runtime_error("failed to allocate descriptor sets!");

        for (uint32_t i = 0; i < framesInFlight; i++)
        {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = uniformBuffer.buffer(i);
            bufferInfo.offset = 0;
            bufferInfo.range = uniformBuffer.size();

            VkWriteDescriptorSet descriptorWrite{};
            descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrite.dstSet = descriptorSets[i];
            descriptorWrite.dstBinding = 0; // matches layout(set = 0, binding = 0) in your shader
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            vkUpdateDescriptorSets(device.device, 1, &descriptorWrite, 0, nullptr);
        }
    }
}