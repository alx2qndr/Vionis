#pragma once

#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.h>

namespace vionis::vulkan
{

class WindowSurface
{
public:
    WindowSurface(SDL_Window *window, VkInstance instance, const VkAllocationCallbacks *allocationCallbacks = nullptr);
    ~WindowSurface();

    VkSurfaceKHR get() const { return m_surface; }

private:
    VkInstance m_instance;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    const VkAllocationCallbacks *m_allocationCallbacks;
};

} // namespace vionis::Vulkan