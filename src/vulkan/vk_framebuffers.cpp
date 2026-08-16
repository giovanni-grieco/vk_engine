#include "vk_framebuffers.hpp"

namespace engine{
    VulkanFramebuffers::VulkanFramebuffers(VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass){
        init(device, swapChain, renderPass);
    }

    VulkanFramebuffers::~VulkanFramebuffers(){
        destroy();
    }

    void VulkanFramebuffers::recreate(VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass){
        destroy();
        init(device, swapChain, renderPass);
    }

    void VulkanFramebuffers::init(VulkanDevice& device, VulkanSwapChain& swapChain, VulkanRenderPass& renderPass){
        this->device = device.device;
        frameBuffers.resize(swapChain.imageViews.size());

        for (size_t i = 0; i<swapChain.imageViews.size(); i++){
            VkImageView attachments[] = {
                swapChain.imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChain.extent.width;
            framebufferInfo.height = swapChain.extent.height;
            framebufferInfo.layers = 1;

            if(vkCreateFramebuffer(device.device, &framebufferInfo, nullptr, &frameBuffers[i]) != VK_SUCCESS){
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    void VulkanFramebuffers::destroy(){
        for (auto framebuffer: frameBuffers){
            if(device != VK_NULL_HANDLE && framebuffer != VK_NULL_HANDLE){
                vkDestroyFramebuffer(device, framebuffer, nullptr);
            }
        }
    }
}