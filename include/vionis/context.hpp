#pragma once

#include <vulkan/vulkan.h>

#include <string_view>
#include <vector>

namespace vionis
{

#ifdef NDEBUG
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

class Context
{
public:
    Context() = default;
    Context(std::string_view applicationName, std::vector<const char *> requiredInstanceExtensions);

    Context(const Context &) = delete;
    Context &operator=(const Context &) = delete;

    Context(Context &&) noexcept = default;
    Context &operator=(Context &&) noexcept = delete;

    ~Context();

    VkInstance instance() const { return m_instance; }
    VkDebugUtilsMessengerEXT debugUtilsMessengerEXT() const { return m_debugUtilsMessengerEXT; }

    const std::vector<const char *> validationLayers() const { return m_validationLayers; }

private:
    void createInstance();
    void setupDebugMessenger();

    bool checkValidationLayerSupport();
    void checkRequiredInstanceExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    std::string_view m_applicationName;
    std::vector<const char *> m_requiredInstanceExtensions;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT = VK_NULL_HANDLE;

    const std::vector<const char *> m_validationLayers = {"VK_LAYER_KHRONOS_validation"};
};

} // namespace vionis
