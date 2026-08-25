#version 450

layout(set = 1, binding = 0) uniform sampler2D texSampler;

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragUV;

void main() {
    outColor = texture(texSampler, fragUV) * vec4(fragColor, 1.0);
}