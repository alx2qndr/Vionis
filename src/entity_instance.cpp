#include "vionis/entity_instance.hpp"

#include <numeric>

namespace vionis
{

EntityRegistry::EntityRegistry(Device &device)
{
    int alignment = std::lcm(device.physicalDeviceProperties().limits.nonCoherentAtomSize,
                             device.physicalDeviceProperties().limits.minUniformBufferOffsetAlignment);

    for (int i = 0; i < m_uniformBuffers.size(); i++)
    {
        m_uniformBuffers[i] = std::make_unique<Buffer>(device, sizeof(EntityUniformData), MAX_ENTITIES,
                                                       VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, alignment);
        m_uniformBuffers[i]->map();
    }
}

void EntityRegistry::updateUniformBuffers(int frameIndex)
{
    for (const auto &kv : m_entities)
    {
        const auto &obj = kv.second;
        EntityUniformData data{};
        data.modelMatrix = obj.transformComponent.toMatrix();
        data.normalMatrix = obj.transformComponent.computeNormalMatrix();
        data.baseColor = obj.materialComponent.baseColor;

        m_uniformBuffers[frameIndex]->writeToIndex(&data, kv.first);
    }

    m_uniformBuffers[frameIndex]->flush();
}

// ---------- EntityInstance ----------

VkDescriptorBufferInfo EntityInstance::getUniformBufferInfo(int frameIndex) const
{
    return registryRef.getEntityUniformBufferInfo(id, frameIndex);
}

EntityInstance::EntityInstance(ID objId, const EntityRegistry &registry) : id{objId}, registryRef{registry} {}

} // namespace vionis
