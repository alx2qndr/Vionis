#pragma once

#include "vionis/entity_instance.hpp"
#include "vionis/descriptors.hpp"
#include "vionis/camera.hpp"

#include <vulkan/vulkan.h>

namespace vionis
{

struct GlobalUniformBufferObject
{
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
    alignas(16) glm::vec3 viewPosition;
};

struct FrameInfo
{
    int frameIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    Camera &camera;
    VkDescriptorSet globalDescriptorSet;
    DescriptorPool &frameDescriptorPool;
    EntityInstance::Map &gameObjects;
};

} // namespace vionis
