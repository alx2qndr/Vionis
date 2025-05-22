#pragma once

#include "vionis/device.hpp"

#include <vulkan/vulkan.h>

#include <memory>
#include <string>

namespace vionis
{

class Texture
{
public:
    Texture(Device &device, const std::string &textureFilepath);
    static std::unique_ptr<Texture> createFromFile(Device &device, const std::string &filePath);

    ~Texture();

    Texture(const Texture &) = delete;
    Texture &operator=(const Texture &) = delete;

    VkImageView imageView() const { return m_textureImageView; }
    VkSampler sampler() const { return m_textureSampler; }
    VkImage image() const { return m_textureImage; }
    VkDescriptorImageInfo descriptorInfo() const { return m_descriptor; }
    VkImageLayout imageLayout() const { return m_textureLayout; }
    VkExtent3D extent() const { return m_extent; }
    VkFormat format() const { return m_format; }

    void updateDescriptor();

private:
    void loadImage(const std::string &filepath);
    void createImageView(VkImageViewType viewType);
    void createSampler();
    void generateMipmaps();

    VkDescriptorImageInfo m_descriptor{};
    Device &m_device;
    VkImage m_textureImage = VK_NULL_HANDLE;
    VkDeviceMemory m_textureImageMemory = VK_NULL_HANDLE;
    VkImageView m_textureImageView = VK_NULL_HANDLE;
    VkSampler m_textureSampler = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    VkImageLayout m_textureLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    uint32_t m_mipLevels = 1;
    uint32_t m_layerCount = 1;
    VkExtent3D m_extent = {};
};

} // namespace vionis
