#version 450

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec3 fragColor;
layout(location = 3) in vec2 fragUV;

void main() {
    // Point light positioned in the world.
    vec3 lightPos = vec3(2.0, 3.0, 2.0);

    vec3 N = normalize(fragNormal);
    vec3 baseColor = texture(texSampler, fragUV).rgb * fragColor;

    //vec3 baseColor(1.0, 1.0, 1.0);

    vec3 toLight = lightPos - fragPos;
    float dist = length(toLight);
    vec3 L = toLight / dist;

    float diffuse = max(dot(N, L), 0.0);
    float attenuation = 1.0 / (1.0 + 0.2 * dist * dist);

    const float ambient = 0.01;
    vec3 lit = baseColor * (ambient + diffuse * attenuation);
    outColor = vec4(lit, 1.0);
}