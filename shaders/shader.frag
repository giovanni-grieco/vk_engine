#version 450

#define MAX_POINT_LIGHTS 16

struct PointLight {
    vec4 positionAndIntensity;
    vec4 colorAndRadius;
};

struct AmbientLight{
    vec4 colorAndIntensity;
};

struct DirectionalLight{
    vec4 directionAndIntensity;
    vec4 colorAndRadius;
};

layout(std430, set = 0, binding = 1) readonly buffer LightBuffer {
    uint lightCount;
    PointLight lights[MAX_POINT_LIGHTS];
    DirectionalLight directionalLight;
    AmbientLight ambientLight;

} lightBlock;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in vec2 fragUV;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 baseColor = texture(texSampler, fragUV).rgb * fragColor;

    vec3 totalLight = vec3(0.0);
    for (uint i = 0; i < lightBlock.lightCount; ++i) {
        PointLight light = lightBlock.lights[i];

        vec3 toLight = light.positionAndIntensity.xyz - fragPos;
        float dist = length(toLight);
        vec3 L = toLight / dist;

        float attenuation = 1.0 / (1.0 + 0.2 * dist * dist);
        totalLight += light.colorAndRadius.rgb
                    * light.positionAndIntensity.w
                    * max(dot(N, L), 0.0)
                    * attenuation;
    }

    vec3 lit = baseColor * (lightBlock.ambientLight.colorAndIntensity.w + totalLight);
    outColor = vec4(lit, 1.0);
}