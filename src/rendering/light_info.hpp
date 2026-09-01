#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

namespace engine
{

    constexpr uint32_t MAX_POINT_LIGHTS = 16;

    struct PointLightInfo
    {
        glm::vec3 position{};
        glm::vec3 color{};
        float intensity = 0.0f;
    };

    struct DirectionalLightInfo
    {
        glm::vec3 direction{};
        glm::vec3 color{};
        float intensity = 0.0f;
    };

    struct AmbientLightInfo
    {  
        glm::vec3 color{};
        float intensity = 0.0f;
    };

    struct LightInfo
    {
        AmbientLightInfo ambient{};
        DirectionalLightInfo directional {};
        std::vector<PointLightInfo> pointLights {};
    };
}