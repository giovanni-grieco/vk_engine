#include "vk_descriptor_sets.hpp"
#include <stdexcept>

namespace engine
{
    VulkanDescriptorSets::VulkanDescriptorSets(VulkanDevice &device,
                                               VulkanDescriptorSetLayout &layout,
                                               VulkanDescriptorPool &pool,
                                               VulkanUniformBuffer &uniformBuffer,
                                               VulkanUniformBuffer &lightBuffer,
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
            VkDescriptorBufferInfo uboInfo{};
            uboInfo.buffer = uniformBuffer.buffer(i);
            uboInfo.offset = 0;
            uboInfo.range = uniformBuffer.size();

            VkDescriptorBufferInfo lightInfo{};
            lightInfo.buffer = lightBuffer.buffer(i);
            lightInfo.offset = 0;
            lightInfo.range = lightBuffer.size();

            VkWriteDescriptorSet descriptorWrites[2]{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0; // layout(set = 0, binding = 0) — camera UBO
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &uboInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1; // layout(set = 0, binding = 1) — point-light SSBO
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pBufferInfo = &lightInfo;

            vkUpdateDescriptorSets(device.device, 2, descriptorWrites, 0, nullptr);
        }
    }
}