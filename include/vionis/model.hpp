#pragma once

#include "vionis/buffer.hpp"
#include "vionis/device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace vionis
{

class Model
{
public:
    struct Vertex
    {
        glm::vec3 position{};
        glm::vec3 color{};
        glm::vec3 normal{};
        glm::vec2 uv{};

        static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

        bool operator==(const Vertex &other) const
        {
            return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
        }
    };

    Model(Device &device, const std::string &filepath);
    static std::unique_ptr<Model> createFromFile(Device &device, const std::string &filePath);

    ~Model();

    Model(const Model &) = delete;
    Model &operator=(const Model &) = delete;

    void bind(VkCommandBuffer commandBuffer);
    void draw(VkCommandBuffer commandBuffer);

private:
    void createVertexBuffers(const std::vector<Vertex> &vertices);
    void createIndexBuffers(const std::vector<uint32_t> &indices);

    Device &device;

    std::unique_ptr<Buffer> vertexBuffer;
    uint32_t vertexCount;

    bool hasIndexBuffer = false;
    std::unique_ptr<Buffer> indexBuffer;
    uint32_t indexCount;
};

} // namespace vionis
