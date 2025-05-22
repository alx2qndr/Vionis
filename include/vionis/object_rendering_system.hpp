#pragma once

#include "vionis/device.hpp"
#include "vionis/frame_info.hpp"
#include "vionis/pipeline.hpp"

#include <memory>

namespace vionis
{
class ObjectRenderingSystem
{
public:
    ObjectRenderingSystem(Device &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
    ~ObjectRenderingSystem();

    ObjectRenderingSystem(const ObjectRenderingSystem &) = delete;
    ObjectRenderingSystem &operator=(const ObjectRenderingSystem &) = delete;

    void renderGameObjects(FrameInfo &frameInfo);

private:
    void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
    void createPipeline(VkRenderPass renderPass);

    Device &device;

    std::unique_ptr<Pipeline> pipeline;
    VkPipelineLayout pipelineLayout;

    std::unique_ptr<DescriptorSetLayout> renderSystemLayout;
};

} // namespace vionis
