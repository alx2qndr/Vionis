#version 450

layout(location = 0) in vec3 inColor;
layout(location = 1) in vec3 inWorldPosition;
layout(location = 2) in vec3 inNormalWorld;
layout(location = 3) in vec2 inUVCoordinate;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    mat4 projection;
    mat4 view;
    vec3 viewPosition;
} ubo;

layout(set = 1, binding = 0) uniform GameObjectBufferData {
    mat4 modelMatrix;
    mat4 normalMatrix;
    vec3 baseColor;
} gameObject;

layout(set = 1, binding = 1) uniform sampler2D diffuseSampler2D;

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec3 textureColor = texture(diffuseSampler2D, inUVCoordinate).rgb;
    vec3 finalColor = vec3(textureColor * gameObject.baseColor);

    outColor = vec4(finalColor, 1.0);
}
