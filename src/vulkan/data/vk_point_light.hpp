#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace engine
{
    // Upper bound on the light list; must match MAX_POINT_LIGHTS in shader.frag.
    constexpr uint32_t MAX_POINT_LIGHTS = 16;

    // Must match the PointLight struct in shader.frag (std430 layout).
    struct PointLightGPU
    {
        glm::vec4 positionAndIntensity; // xyz = world position, w = intensity
        glm::vec4 colorAndRadius;       // rgb = color, w = unused
    };

    // std430 layout of the SSBO: uint count, 12 bytes of padding to align the
    // array to 16 bytes, then the light array.
    struct LightBufferData
    {
        uint32_t lightCount = 0;
        uint32_t _pad0 = 0;
        uint32_t _pad1 = 0;
        uint32_t _pad2 = 0;
        PointLightGPU lights[MAX_POINT_LIGHTS];
    };
}
