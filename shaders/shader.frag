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
layout(location = 4) flat in int fragHasTexture;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 texel = vec3(1.0);
    if (fragHasTexture == 1) {
        texel = texture(texSampler, fragUV).rgb;
    }
    vec3 baseColor = texel * fragColor;

    vec3 lighting = lightBlock.ambientLight.colorAndIntensity.rgb
                    * lightBlock.ambientLight.colorAndIntensity.w;

    uint count = min(lightBlock.lightCount, MAX_POINT_LIGHTS);
    for (uint i = 0; i < count; ++i) {
        PointLight light = lightBlock.lights[i];

        vec3 toLight = light.positionAndIntensity.xyz - fragPos;
        float dist = length(toLight);
        vec3 L = toLight / dist;

        float attenuation = 1.0 / (1.0 + 0.2 * dist * dist);
        float NdotL = max(dot(N, L), 0.0);

        lighting += light.colorAndRadius.rgb
                  * light.positionAndIntensity.w
                  * NdotL
                  * attenuation;
    }

    vec3 Ld = normalize(-lightBlock.directionalLight.directionAndIntensity.xyz);
    float NdotLd = max(dot(N, Ld), 0.0);
    lighting += lightBlock.directionalLight.colorAndRadius.rgb
              * lightBlock.directionalLight.directionAndIntensity.w
              * NdotLd;

    vec3 finalColor = baseColor * lighting;
    outColor = vec4(finalColor, 1.0);
}