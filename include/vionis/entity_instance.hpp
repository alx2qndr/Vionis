#pragma once

#include "vionis/components/material_component.hpp"
#include "vionis/components/transform_component.hpp"
#include "vionis/swapchain.hpp"
#include "vionis/model.hpp"
#include "vionis/texture.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <cassert>
#include <memory>
#include <unordered_map>

namespace vionis
{

// ---------- EntityUniformData ----------

struct EntityUniformData
{
    glm::mat4 modelMatrix;
    glm::mat4 normalMatrix;
    glm::vec3 baseColor;
};

// ---------- EntityInstance forward declaration ----------

class EntityRegistry;

class EntityInstance
{
public:
    using ID = uint32_t;
    using Map = std::unordered_map<ID, EntityInstance>;

    EntityInstance(const EntityInstance &) = delete;
    EntityInstance &operator=(const EntityInstance &) = delete;

    EntityInstance(EntityInstance &&) = default;
    EntityInstance &operator=(EntityInstance &&) = delete;

    ID getId() const { return id; }

    VkDescriptorBufferInfo getUniformBufferInfo(int frameIndex) const;

    TransformComponent transformComponent;
    MaterialComponent materialComponent;
    std::shared_ptr<Model> model;
    std::shared_ptr<Texture> diffuseTexture;

private:
    EntityInstance(ID entityId, const EntityRegistry &registry);

    ID id;
    const EntityRegistry &registryRef;

    friend class EntityRegistry;
};

// ---------- EntityRegistry ----------

class EntityRegistry
{
public:
    explicit EntityRegistry(Device &device);

    EntityRegistry(const EntityRegistry &) = delete;
    EntityRegistry &operator=(const EntityRegistry &) = delete;

    EntityRegistry(EntityRegistry &&) = delete;
    EntityRegistry &operator=(EntityRegistry &&) = delete;

    EntityInstance &createEntity()
    {
        assert(m_nextId < MAX_ENTITIES && "Max entity count exceeded!");
        EntityInstance entity{m_nextId++, *this};
        entity.diffuseTexture = m_defaultDiffuseTexture;
        m_entities.emplace(entity.getId(), std::move(entity));
        return m_entities.at(entity.getId());
    }

    VkDescriptorBufferInfo getEntityUniformBufferInfo(EntityInstance::ID entityId, int frameIndex) const
    {
        return m_uniformBuffers[frameIndex]->descriptorInfoForIndex(entityId);
    }

    void updateUniformBuffers(int frameIndex);

    static constexpr int MAX_ENTITIES = 100000;
    EntityInstance::Map &entities() { return m_entities; }

private:
    EntityInstance::ID m_nextId{0};
    EntityInstance::Map m_entities;
    std::vector<std::unique_ptr<Buffer>> m_uniformBuffers{Swapchain::MAX_FRAMES_IN_FLIGHT};
    std::shared_ptr<Texture> m_defaultDiffuseTexture;
};

// // ---------- EntityUboArraySystem ----------
//
// template <typename T>
// class EntityUboArraySystem
// {
// public:
//     static constexpr int MAX_ENTITIES = 1000;
//
//     EntityUboArraySystem(Device &device, int length)
//     {
//         int alignment = std::lcm(device.properties.limits.nonCoherentAtomSize,
//                                  device.properties.limits.minUniformBufferOffsetAlignment);
//
//         for (auto &buffer : uboBuffers)
//         {
//             buffer = std::make_unique<Buffer>(device, sizeof(T), length, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
//                                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, alignment);
//             buffer->map();
//         }
//     }
//
//     EntityUboArraySystem(const EntityUboArraySystem &) = delete;
//     EntityUboArraySystem &operator=(const EntityUboArraySystem &) = delete;
//
//     EntityUboArraySystem(EntityUboArraySystem &&) = delete;
//     EntityUboArraySystem &operator=(EntityUboArraySystem &&) = delete;
//
//     void updateBuffer(int frameIndex, const std::unordered_map<EntityInstance::ID, T> &uboDataMap)
//     {
//         assert(frameIndex < uboBuffers.size());
//
//         for (const auto &[entityId, data] : uboDataMap)
//         {
//             if (entityIndexMap.find(entityId) == entityIndexMap.end())
//             {
//                 entityIndexMap[entityId] = static_cast<int>(entityIndexMap.size());
//                 assert(entityIndexMap[entityId] < MAX_ENTITIES && "Exceeded maximum UBO array size!");
//             }
//
//             int index = entityIndexMap[entityId];
//             uboBuffers[frameIndex]->writeToIndex(&data, index);
//         }
//     }
//
//     VkDescriptorBufferInfo getDescriptorInfo(EntityInstance::ID entityId, int frameIndex) const
//     {
//         auto it = entityIndexMap.find(entityId);
//         assert(it != entityIndexMap.end() && "Entity ID not found in UBO system!");
//         return uboBuffers[frameIndex]->descriptorInfoForIndex(it->second);
//     }
//
// private:
//     std::unordered_map<EntityInstance::ID, int> entityIndexMap;
//     std::vector<std::unique_ptr<Buffer>> uboBuffers{Swapchain::MAX_FRAMES_IN_FLIGHT};
// };

} // namespace vionis
