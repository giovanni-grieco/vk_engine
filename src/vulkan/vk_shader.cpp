#include "vk_shader.hpp"

namespace engine{

    VulkanShader::VulkanShader(VulkanDevice& device, const std::vector<char>& code, VulkanShaderType type){
        this->device = device.device;
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
        if(vkCreateShaderModule(device.device, &createInfo, nullptr, &shaderModule)!= VK_SUCCESS){
            throw std::runtime_error("Failed to create shader module");
        }
    }

    VulkanShader::~VulkanShader(){
        vkDestroyShaderModule(device, shaderModule, nullptr);
    }

}