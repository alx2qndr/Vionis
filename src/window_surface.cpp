#include "vionis/window_surface.hpp"

#include <stdexcept>
#include <string>

namespace vionis::vulkan
{

WindowSurface::WindowSurface(SDL_Window *window, VkInstance instance, const VkAllocationCallbacks *allocationCallbacks)
    : m_instance(instance), m_allocationCallbacks(allocationCallbacks)
{
    if (!SDL_Vulkan_CreateSurface(window, instance, allocationCallbacks, &m_surface))
    {
        throw std::runtime_error("Failed to create Vulkan surface: " + std::string(SDL_GetError()));
    }
}

WindowSurface::~WindowSurface()
{
    if (m_instance != VK_NULL_HANDLE && m_surface != VK_NULL_HANDLE)
    {
        SDL_Vulkan_DestroySurface(m_instance, m_surface, m_allocationCallbacks);
    }
}

} // namespace vionis::vulkan