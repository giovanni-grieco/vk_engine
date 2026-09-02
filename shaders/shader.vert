#version 450

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform PushConstants {
    mat4 model;
    vec4 normal[3];
    vec4 color;
} pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUV;

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec3 fragColor;
layout(location = 3) out vec2 fragUV;

void main() {

    vec4 worldPos = pc.model * vec4(inPosition, 1.0);
    vec3 worldNormal = normalize(mat3(pc.normal[0].xyz, pc.normal[1].xyz, pc.normal[2].xyz) * inNormal);

    gl_Position = ubo.proj * ubo.view * worldPos;

    fragPos = vec3(worldPos);
    fragNormal = worldNormal;
    fragColor = inColor * pc.color.rgb;
    fragUV = inUV;
}