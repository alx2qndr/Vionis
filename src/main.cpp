#include "vionis/vionis.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

int main(int argc, char **argv)
{
    try
    {
        vionis::Window window(1024, 576, "Hello, vionis Window!");
        vionis::Context context("Unnamed Application", vionis::Window::getRequiredInstanceExtensions());
        vionis::Device device(context, window);
        vionis::Renderer renderer(window, device);

        std::unique_ptr<vionis::DescriptorPool> globalPool =
            vionis::DescriptorPool::Builder(device)
                .setMaxSets(vionis::Swapchain::MAX_FRAMES_IN_FLIGHT)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vionis::Swapchain::MAX_FRAMES_IN_FLIGHT)
                .build();

        std::vector<std::unique_ptr<vionis::DescriptorPool>> framePools(vionis::Swapchain::MAX_FRAMES_IN_FLIGHT);
        auto framePoolBuilder =
            vionis::DescriptorPool::Builder(device)
                .setMaxSets(vionis::EntityRegistry::MAX_ENTITIES)
                .addPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, vionis::EntityRegistry::MAX_ENTITIES)
                .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, vionis::EntityRegistry::MAX_ENTITIES)
                .setPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT);
        for (int i = 0; i < framePools.size(); i++)
        {
            framePools[i] = framePoolBuilder.build();
        }

        vionis::EntityRegistry entityRegistry{device};

        std::shared_ptr<vionis::Model> model =
            vionis::Model::createFromFile(device, "../assets/models/tiny_frog/model.obj");

        std::shared_ptr<vionis::Texture> texture =
            vionis::Texture::createFromFile(device, "../assets/models/tiny_frog/textures/baseColor.png");

        auto &tinyFrog = entityRegistry.createEntity();
        tinyFrog.model = model;
        tinyFrog.diffuseTexture = texture;

        tinyFrog.transformComponent.position = {0.0f, 0.0f, 0.0f};
        tinyFrog.transformComponent.scale = {1.0f, 1.0f, 1.0f};
        tinyFrog.transformComponent.rotation = {0.0f, 0.0f, 0.0f};

        tinyFrog.materialComponent.baseColor = {0.8f, 0.8f, 0.8f};

        std::vector<std::unique_ptr<vionis::Buffer>> uboBuffers(vionis::Swapchain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<vionis::Buffer>(device, sizeof(vionis::GlobalUniformBufferObject), 1,
                                                             VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = vionis::DescriptorSetLayout::Builder(device)
                                   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
                                   .build();

        std::vector<VkDescriptorSet> globalDescriptorSets(vionis::Swapchain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            vionis::DescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globalDescriptorSets[i]);
        }

        vionis::ObjectRenderingSystem simpleRenderSystem{device, renderer.getSwapchainRenderPass(),
                                                         globalSetLayout->getDescriptorSetLayout()};

        vionis::Camera camera{};
        auto &viewerObject = entityRegistry.createEntity();
        viewerObject.transformComponent.position = {1.0f, 1.0f, 1.0f};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!window.closeRequested())
        {
            window.processEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(75.f, aspect, 0.1f, 4096.0f);
            camera.setViewTarget(viewerObject.transformComponent.position, glm::vec3(0.0f, 0.0f, 0.0f));

            if (auto commandBuffer = renderer.beginFrame())
            {
                int frameIndex = renderer.getFrameIndex();
                framePools[frameIndex]->resetPool();
                vionis::FrameInfo frameInfo{frameIndex,
                                            frameTime,
                                            commandBuffer,
                                            camera,
                                            globalDescriptorSets[frameIndex],
                                            *framePools[frameIndex],
                                            entityRegistry.entities()};

                vionis::GlobalUniformBufferObject ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.viewPosition = camera.getPosition();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                entityRegistry.updateUniformBuffers(frameIndex);

                renderer.beginSwapchainRenderPass(commandBuffer);

                simpleRenderSystem.renderGameObjects(frameInfo);

                renderer.endSwapchainRenderPass(commandBuffer);
                renderer.endFrame();
            }
        }

        vkDeviceWaitIdle(device.device());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
