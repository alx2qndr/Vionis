#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec2 inUVCoordinate;

layout(location = 0) out vec3 outColor;
layout(location = 1) out vec3 outWorldPosition;
layout(location = 2) out vec3 outNormalWorld;
layout(location = 3) out vec2 outUVCoordinate;

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

layout(push_constant) uniform Push {
    mat4 modelMatrix;
    mat4 normalMatrix;
} push;

void main() {
    vec4 positionWorld = gameObject.modelMatrix * vec4(inPosition, 1.0);
    gl_Position = ubo.projection * ubo.view * positionWorld;
    outNormalWorld = normalize(mat3(gameObject.normalMatrix) * inNormal);
    outWorldPosition = positionWorld.xyz;
    outColor = inColor;
    outUVCoordinate = inUVCoordinate;
}
