#pragma once

#include <string>
#include <vector>

#include "vionis/context.hpp"
#include "vionis/window.hpp"

namespace vionis
{

struct SwapchainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;

    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

class Device
{
public:
    Device(Context &context, Window &window);
    ~Device();

    Device(const Device &) = delete;
    Device &operator=(const Device &) = delete;
    Device(Device &&) = delete;
    Device &operator=(Device &&) = delete;

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);
    void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image,
                             VkDeviceMemory &imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
                               uint32_t mipLevels = 1, uint32_t layerCount = 1);

    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling,
                                 VkFormatFeatureFlags features);
    QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(m_physicalDevice); }
    SwapchainSupportDetails getSwapchainSupport() { return querySwapchainSupport(m_physicalDevice); }

    float getMaxAnisotropy() const { return m_physicalDeviceProperties.limits.maxSamplerAnisotropy; }

    bool supportsAnisotropy() const { return m_physicalDeviceFeatures.samplerAnisotropy == VK_TRUE; }

    VkDevice device() { return m_device; }
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkPhysicalDeviceProperties physicalDeviceProperties() const { return m_physicalDeviceProperties; };
    VkPhysicalDeviceFeatures physicalDeviceFeatures() const { return m_physicalDeviceFeatures; };
    VkCommandPool getCommandPool() { return m_commandPool; }
    VkQueue graphicsQueue() { return m_graphicsQueue; }
    VkQueue presentQueue() { return m_presentQueue; }
    VkSurfaceKHR surface() { return m_surface->get(); }

#ifdef NDEBUG
    const bool enableValidationLayers = false;
#else
    const bool enableValidationLayers = true;
#endif

private:
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createCommandPool();

    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
    SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device);

    std::string physicalDeviceTypeToString(VkPhysicalDeviceType type) const;
    int rateDeviceSuitability(VkPhysicalDevice device);

    Context &m_context;
    Window &m_window;

    std::unique_ptr<vulkan::WindowSurface> m_surface;

    VkDevice m_device = VK_NULL_HANDLE;

    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties m_physicalDeviceProperties;
    VkPhysicalDeviceFeatures m_physicalDeviceFeatures;

    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkQueue m_presentQueue = VK_NULL_HANDLE;

    VkCommandPool m_commandPool = VK_NULL_HANDLE;

    const std::vector<const char *> m_deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
};

} // namespace vionis
